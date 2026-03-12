import pandas as pd
from pathlib import Path

from numpy.f2py.auxfuncs import throw_error
from scipy.signal import find_peaks
from sklearn.preprocessing import normalize
from pybaselines import Baseline
import numpy as np

import joblib
from sklearn.preprocessing import LabelEncoder, OneHotEncoder

import os

def add_filename_features(df, filename):
    try:
        parts = Path(filename).stem.split('_')
        df = df.copy()

        keywords = ['@']

        split_idx = next(
            (i for i, p in enumerate(parts) if any(k in p.lower() for k in keywords)),
            len(parts)
        )

        df['position'] = '_'.join(parts[:split_idx])

        if split_idx < len(parts):
            df['class'] = parts[split_idx]
            start_idx = split_idx + 1
        else:
            start_idx = split_idx

        index_param = 0
        names = ['group', 'nm', 'center', 'obj', 'power', 'during', 'acc', 'map', 'step', 'place']

        for i, part in enumerate(parts[start_idx:], start=start_idx + 1):

            if index_param >= 10:
                df[names[-1]] = df[names[-1]] + '_' + part.replace(names[-1], '')
            else:
                df[names[index_param]] = part.replace(names[index_param], '')
            index_param += 1

        if index_param != 10 and index_param != 11:
            print(f"\nНазвание файла: {filename}")
            throw_error("Ошибка в названии файла. Встречено не 12 параметров!")

        return df

    except Exception as e:
        print("\nERROR in add_filename_features")
        print("filename:", filename)
        print("exception type:", type(e).__name__)
        print("message:", e)
        print("\nTraceback:")

        raise



def convert_spectra_file_to_csv(input_filepath, output_filepath=None):
    # Читаем файл с пропуском строк, начинающихся с #
    df_raw = pd.read_csv(input_filepath, sep='\t', comment='#',
                         names=['X', 'Y', 'Wave', 'Intensity'])

    # Получаем уникальные значения Wave для сортировки
    unique_waves = sorted(df_raw['Wave'].unique(), reverse=True)
    n_waves = len(unique_waves)

    print(f"Найдено уникальных волн: {n_waves}")
    print(f"Диапазон волн: от {unique_waves[-1]:.3f} до {unique_waves[0]:.3f}")

    # Создаем сводную таблицу
    df_pivot = df_raw.pivot_table(
        index=['X', 'Y'],
        columns='Wave',
        values='Intensity',
        aggfunc='first'  # Берем первое значение (должно быть только одно)
    )


    # Сбрасываем индекс, чтобы X и Y стали колонками
    df_result = df_pivot.reset_index()

    df_result = add_filename_features(df_result, input_filepath)

    meta_cols = list(df_result.columns[-12:])
    xy_cols = [df_result.columns[0], df_result.columns[1]]
    wave_cols = list(df_result.columns[2:-12])

    new_order = meta_cols + xy_cols + wave_cols

    df_result = df_result[new_order]

    df_result.columns = (
        meta_cols +
        xy_cols +
        [f'Wave_{float(col):.1f}' for col in wave_cols]
    )

    # Сохраняем в CSV
    if output_filepath is None:
        input_path = Path(input_filepath)
        output_filepath = input_path.with_suffix('.csv')

    df_result.to_csv(output_filepath, index=False)
    print(f"Файл сохранен: {output_filepath}")
    print(f"Размерность результирующего dataframe: {df_result.shape}")

    return df_result



def load_dataset(csv_path: str):
    df = pd.read_csv(csv_path, low_memory=False)

    wave_cols = [c for c in df.columns if str(c).startswith("Wave_")]
    if len(wave_cols) != 1015:
        raise ValueError(f"В файле {csv_path} не найдены 1015 Wave_* колонок\nДанные не походят по размерности!\n Прверьте имя файла, вместо класса должен стоять символ @")

    def parse_wave(c):
        return float(str(c).split("_", 1)[1])

    wave_axis = np.array([parse_wave(c) for c in wave_cols], dtype=float)
    order = np.argsort(wave_axis)
    wave_axis = wave_axis[order]
    wave_cols = [wave_cols[i] for i in order]

    #keep_meta = [c for c in ["position", "class", "group", "place", "X", "Y"] if c in df.columns]
    #df = df[keep_meta + wave_cols].copy()

    for c in wave_cols:
        df[c] = pd.to_numeric(df[c], errors="coerce")

    if "X" in df.columns:
        df["X"] = pd.to_numeric(df["X"], errors="coerce")
    if "Y" in df.columns:
        df["Y"] = pd.to_numeric(df["Y"], errors="coerce")

    return df, wave_axis, wave_cols


def build_windows_for_axis_by_peaks(
    df,
    wave_axis,
    wave_cols,
    n_peaks=10,
    half_width=50.0,
    min_distance=None,
):
    Xmat = df[wave_cols].to_numpy(dtype=float)
    spectrum = pd.Series(np.nanmean(Xmat, axis=0)).ffill().bfill().to_numpy()

    if min_distance is None:
        min_distance = len(wave_axis) // 50

    peaks, props = find_peaks(spectrum, distance=min_distance, prominence=0)

    if len(peaks) == 0:
        raise ValueError("No peaks found.")

    prominences = props["prominences"]
    n_peaks = min(len(peaks), n_peaks)
    top_idx = np.argsort(prominences)[-n_peaks:]
    top_peaks = peaks[top_idx]
    top_prom = prominences[top_idx]

    windows = [
        (
            float(wave_axis[p] - half_width),
            float(wave_axis[p] + half_width),
            f"P{i}_{wave_axis[p]:.1f}"
        )
        for i, p in enumerate(top_peaks, start=1)
    ]

    return windows, top_peaks, top_prom, spectrum

def print_windows(windows):
    print("\nНайденые пики на входных данных:\n")
    for i, (lo, hi, name) in enumerate(windows, 1):
        center = (lo + hi) / 2
        width = hi - lo
        print(f"{i:2d}. {name:15}  center={center:8.2f}  [{lo:8.2f}, {hi:8.2f}]  width={width:.2f}")


def build_window_features(df, wave_axis, wave_cols, prefix=""):
    Xmat = df[wave_cols].to_numpy(dtype=float)

    windows, top_peaks, top_prom, mean_spectrum = build_windows_for_axis_by_peaks(
        df=df,
        wave_axis=wave_axis,
        wave_cols=wave_cols,
        n_peaks=10,
        half_width=50,
    )

    print_windows(windows)

    feat = {}

    # Глобальные статистики
    feat[f"{prefix}Global_min"] = np.min(Xmat, axis=1)
    feat[f"{prefix}Global_max"] = np.max(Xmat, axis=1)
    feat[f"{prefix}Global_range"] = feat[f"{prefix}Global_max"] - feat[f"{prefix}Global_min"]
    feat[f"{prefix}Global_mean"] = np.mean(Xmat, axis=1)
    feat[f"{prefix}Global_std"] = np.std(Xmat, axis=1)
    feat[f"{prefix}Global_median"] = np.median(Xmat, axis=1)

    window_names = []
    for lo, hi, name in windows:
        mask = (wave_axis >= lo) & (wave_axis <= hi)
        Xm = Xmat[:, mask]

        nm = f"{prefix}{name}"
        window_names.append(nm)

        feat[nm] = np.trapezoid(Xm, x=wave_axis[mask], axis=1)

        feat[f"{prefix}Min_{name}"] = np.min(Xm, axis=1)
        feat[f"{prefix}Max_{name}"] = np.max(Xm, axis=1)
        feat[f"{prefix}Range_{name}"] = feat[f"{prefix}Max_{name}"] - feat[f"{prefix}Min_{name}"]

        feat[f"{prefix}Mean_{name}"] = np.mean(Xm, axis=1)
        feat[f"{prefix}Std_{name}"] = np.std(Xm, axis=1)
        feat[f"{prefix}Median_{name}"] = np.median(Xm, axis=1)

    F = pd.DataFrame(feat).ffill().bfill()
    F = F.fillna(F.mean(numeric_only=True))

    eps = 1e-12
    # Переводим в numpy
    W = {k: F[k].to_numpy() for k in window_names}
    sumW = np.sum(np.column_stack([W[k] for k in window_names]), axis=1) + eps

    for k in window_names:
        F[f"{prefix}Frac_{k}"] = (W[k] + eps) / sumW

    F[f"{prefix}SquareUP"] = np.trapezoid(Xmat, axis=1)

    return F




def build_derivative_features(df, wave_axis, wave_cols, prefix="D_"):
    Xmat = df[wave_cols].to_numpy(dtype=float)
    dX = np.gradient(Xmat, wave_axis, axis=1)

    windows, top_peaks, top_prom, mean_spectrum = build_windows_for_axis_by_peaks(
        df=df,
        wave_axis=wave_axis,
        wave_cols=wave_cols,
        n_peaks=10,
        half_width=50,
    )
    feat = {}

    feat[f"{prefix}Global_min"] = np.min(dX, axis=1)
    feat[f"{prefix}Global_max"] = np.max(dX, axis=1)
    feat[f"{prefix}Global_range"] = feat[f"{prefix}Global_max"] - feat[f"{prefix}Global_min"]

    feat[f"{prefix}Global_mean"] = np.mean(dX, axis=1)
    feat[f"{prefix}Global_std"] = np.std(dX, axis=1)
    feat[f"{prefix}Global_median"] = np.median(dX, axis=1)

    for lo, hi, name in windows:
        mask = (wave_axis >= lo) & (wave_axis <= hi)
        Xm = dX[:, mask]

        feat[f"{prefix}{name}"] = np.trapezoid(np.abs(Xm), x=wave_axis[mask], axis=1)
        feat[f"{prefix}{name}_min"] = np.min(Xm, axis=1)
        feat[f"{prefix}{name}_max"] = np.max(Xm, axis=1)
        feat[f"{prefix}{name}_range"] = np.max(Xm, axis=1) - np.min(Xm, axis=1)
        feat[f"{prefix}{name}_mean"] = np.mean(Xm, axis=1)
        feat[f"{prefix}{name}_std"] = np.std(Xm, axis=1)
        feat[f"{prefix}{name}_median"] = np.median(Xm, axis=1)

    F = pd.DataFrame(feat).ffill().bfill()
    F = F.fillna(F.mean(numeric_only=True))
    return F




def preprocess_wave_features(df):

    wave_cols = [c for c in df.columns if c.startswith("Wave_")]
    X = df[wave_cols].to_numpy(dtype=float)

    baseline_fitter = Baseline()

    X_corr = np.zeros_like(X)

    try:
        for i, spectrum in enumerate(X):
            baseline, _ = baseline_fitter.asls(spectrum, lam=1e5, p=0.01)
            X_corr[i] = spectrum - baseline

        X_norm = normalize(X_corr, norm="l2")

    except Exception as e:
        print("\nОшибка типа ValueError: array must not contain infs or NaNs\n")
        print("Зачастую происходит из-за переполнения буфера ввода\n")
        print("Воспользуйтесь загрузкой данных через файл\n")

        raise

    df[wave_cols] = X_norm
    return df


def produce_feat(path, output):
    df, wave_axis, wave_cols = load_dataset(path)

    df = preprocess_wave_features(df)

    new_features = build_window_features(df, wave_axis, wave_cols)
    df = pd.concat([df, new_features], axis=1)

    new_features = build_derivative_features(df, wave_axis, wave_cols)
    df = pd.concat([df, new_features], axis=1)

    df.to_csv(output, index=False, header=None)


# =============================================================================

# os.chdir("/")


def select_or_create_input_file():
    print("Название файла должно соответствовать следующей структуре:\n")
    print("{brain_region}_@_{group}_{wavelength}_center{center}_obj{obj}_power{power}_{time}_{acc}_map{map_size}_{step}_{place}.txt\nВажно указать @ вместо класса!\n")
    file_path = input("Введите путь к файлу: ").strip()
    if os.path.exists(file_path):
        return file_path
    else:
        print("Файл не найден!")
        return None




if __name__ == "__main__":
    # Получение input_file
    #input_file = "cortex_left_@_2Agroup_633nm_center1500_obj100_power100_1s_5acc_map35x15_step2_place1_1.txt" #select_or_create_input_file()
    input_file = select_or_create_input_file()


    if input_file:
        # ====================ПОДГОТОВКА ФАЙЛА, ПРИВИДЕНИЕ К НУЖНОМУ ФОРМАТУ==============================
        print(f"\nИспользуется файл: {input_file}")
        print("\nВ файле не должны фигурировать названия колонок или быть закоментированы с помощью #\n Разделение между колонками - табуляция, колонок две - 'Wave', 'Intensity'\n")

        df = pd.read_csv(input_file, sep='\t', comment='#', names=['Wave', 'Intensity'])

        # Проверка количества колонок
        if len(df.columns) != 2:
            raise ValueError(f"Файл должен содержать ровно 2 колонки. Обнаружено: {len(df.columns)}")

        print(f"Файл успешно прочитан. Колонки: {list(df.columns)}")

        # Добавление двух новых колонок с нулями в начало
        df.insert(0, '#X', 0)
        df.insert(1, '#Y', 0)

        # Переименование существующих колонок
        df.columns = ['#X', '#Y', '#Wave', '#Intensity']

        # Сохранение во временный файл
        temp_file = input_file.rsplit('.', 1)[0] + ".csv"
        df.to_csv(temp_file, index=False, sep='\t')

        print("Файл успешно обработан. Добавлены колонки #X и #Y с нулями.\n Они не влияют на результат, но должны быть для совместимости предобработки с данными из начального dataset`а\n")

        # ====================ПРЕДОБРАБОТКА======================================

        convert_spectra_file_to_csv(temp_file, 'datasetCsv.csv')

        # Извлечение признаков
        produce_feat('datasetCsv.csv', 'datasetFeat.csv')

        print("\nПредобработка завершена!")

        # ==============================РАБОТА МОДЕЛИ================================


        if 'center1500' in temp_file:
            with open('columns_list_1500.txt', 'r') as f:
                columns = [line.strip() for line in f]
            df0 = pd.read_csv('datasetFeat.csv', names=columns)
            print(df0)

            wave_cols = [c for c in df0.columns if str(c).startswith("Wave_")]


            def parse_wave(c):
                return float(str(c).split("_", 1)[1])


            wave_axis = np.array([parse_wave(c) for c in wave_cols], dtype=float)
            order = np.argsort(wave_axis)
            wave_axis = wave_axis[order]
            wave_cols = [wave_cols[i] for i in order]

            df = df0
            for c in wave_cols:
                df[c] = pd.to_numeric(df[c], errors="coerce")
            df = df.dropna(subset=["position", "class"] + wave_cols).reset_index(drop=True)

            Xmat = df[wave_cols].to_numpy(dtype=float)
            if not np.isfinite(Xmat).all():
                raise ValueError("В интенсивностях есть NaN/inf — сначала почистите данные.")

            region = df["position"].astype(str).to_numpy()
            y_str = df["class"].astype(str).to_numpy()

            le = LabelEncoder()
            y = le.fit_transform(y_str)

            print("Classes:", list(le.classes_))
            print("Samples:", len(df), "| Spectrum points:", Xmat.shape[1], "| Unique regions:", len(np.unique(region)))

            # -------------------- 2) FEATURE ENGINEERING FOR 1500 --------------------

            F = df.drop(
                ["position", "class", "group", "nm", "center", "obj", "power", "during", "acc", "map", "step", "place",
                 "X",
                 "Y"], axis=1)

            X_all = pd.concat([pd.DataFrame({"region": region}), F.reset_index(drop=True)], axis=1)
            feat_names = F.columns.tolist()

            print("Feature matrix:", F.shape)

            checkpoint = joblib.load("hgb_raman_pipeline_1500.joblib")

            model2 = checkpoint["model"]
            le = checkpoint["label_encoder"]
            wave_cols = checkpoint["wave_cols"]

            y_pred_encoded = model2.predict(X_all)

            y_pred = le.inverse_transform(y_pred_encoded)
            print("class: ", y_pred)
            y_pred_proba = model2.predict_proba(X_all)
            print("probability: ", y_pred_proba)

            pass
        elif 'center2900' in temp_file:
            with open('columns_list_2900.txt', 'r') as f:
                columns = [line.strip() for line in f]
            df0 = pd.read_csv('datasetFeat.csv', names=columns)
            print(df0)

            wave_cols = [c for c in df0.columns if str(c).startswith("Wave_")]


            def parse_wave(c):
                return float(str(c).split("_", 1)[1])


            wave_axis = np.array([parse_wave(c) for c in wave_cols], dtype=float)
            order = np.argsort(wave_axis)
            wave_axis = wave_axis[order]
            wave_cols = [wave_cols[i] for i in order]

            df = df0
            for c in wave_cols:
                df[c] = pd.to_numeric(df[c], errors="coerce")
            df = df.dropna(subset=["position", "class"] + wave_cols).reset_index(drop=True)

            Xmat = df[wave_cols].to_numpy(dtype=float)
            if not np.isfinite(Xmat).all():
                raise ValueError("В интенсивностях есть NaN/inf — сначала почистите данные.")

            region = df["position"].astype(str).to_numpy()
            y_str = df["class"].astype(str).to_numpy()

            le = LabelEncoder()
            y = le.fit_transform(y_str)

            print("Classes:", list(le.classes_))
            print("Samples:", len(df), "| Spectrum points:", Xmat.shape[1], "| Unique regions:", len(np.unique(region)))

            # -------------------- 2) FEATURE ENGINEERING FOR 1500 --------------------

            F = df.drop(
                ["position", "class", "group", "nm", "center", "obj", "power", "during", "acc", "map", "step", "place",
                 "X",
                 "Y"], axis=1)

            X_all = pd.concat([pd.DataFrame({"region": region}), F.reset_index(drop=True)], axis=1)
            feat_names = F.columns.tolist()

            print("Feature matrix:", F.shape)

            checkpoint = joblib.load("hgb_raman_pipeline_1500.joblib")

            model2 = checkpoint["model"]
            le = checkpoint["label_encoder"]
            wave_cols = checkpoint["wave_cols"]

            y_pred_encoded = model2.predict(X_all)

            y_pred = le.inverse_transform(y_pred_encoded)
            print("class: ", y_pred)
            y_pred_proba = model2.predict_proba(X_all)
            print("probability: ", y_pred_proba)
            pass
        else:
            print("Неизвестный диапазон волн")

    else:
        print("Не удалось получить input_file.")