//
// Created by pavel on 01.10.2025.
//

#include "gate.h"

#include <algorithm>
#include <ostream>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <new>
#include <cstddef>
#include <string>
#include <utility>
#include <span>

namespace units {

    void Gate::sizeDouble(Pin*& arr, size_t& capacity) {
        auto* ptr = new Pin[capacity * 2];
        std::ranges::move(arr, arr + capacity, ptr);
        capacity *= 2;
        delete [] arr;
        arr = ptr;
    }


    Gate::Gate() : inputs(new Pin[4]), size_in(0), capacity_in(4), size_out(0), capacity_out(4) {
        try {
            outputs = new Pin[capacity_out];
        }
        catch (const std::bad_alloc&) {
            delete [] inputs;
            throw;
        }
    }

    Gate::Gate(const unsigned count_inputs, const unsigned count_outputs) : inputs(nullptr) {
        try {
            inputs = new Pin[count_inputs * 2];
            outputs = new Pin[count_outputs * 2];
        }
        catch (const std::bad_alloc&) {
            delete [] inputs;
            throw;
        }
        size_in = count_inputs;
        capacity_in = size_in * 2;
        size_out = count_outputs;
        capacity_out = size_out * 2;


        std::fill_n(outputs, count_outputs, Pin(Pin::Type::Output));
    }

    Gate::Gate(const std::vector<Pin::State>& in_states, const std::vector<Pin::State>& out_states) :
    inputs(nullptr), size_in(in_states.size()), size_out(in_states.size()), capacity_in(size_in * 2), capacity_out(size_out * 2)  {
        try {
            inputs = new Pin[in_states.size() * 2];
            outputs = new Pin[out_states.size() * 2];
        }
        catch (const std::bad_alloc&) {
            delete [] inputs;
            throw;
        }


        try {
            unsigned i = 0;
            unsigned j = 0;
            for (const auto& state : in_states) {
                inputs[i++] = Pin(Pin::Type::Input, state);
            }

            for (const auto& state : out_states) {
                outputs[j++] = Pin(Pin::Type::Output, state);
            }
        }
        catch (const std::logic_error&) {
            delete [] inputs;
            delete [] outputs;
            throw;
        }
    }

    Gate::Gate(const Gate& g) : inputs(new Pin[g.capacity_in]), size_in(g.size_in), capacity_in(g.capacity_in),
                                size_out(g.size_out), capacity_out(g.capacity_out) {

        try {
            outputs = new Pin[capacity_out];
        }
        catch (const std::bad_alloc&) {
            delete [] inputs;
            throw;
        }

        std::copy_n(g.inputs, size_in, inputs);
        std::copy_n(g.outputs, size_out, outputs);
    }

    Gate::Gate(Gate&& g) noexcept : inputs(g.inputs), size_in(g.size_in), capacity_in(g.capacity_in),
                                    outputs(g.outputs), size_out(g.size_out), capacity_out(g.capacity_out) {
        g.inputs = nullptr;
        g.size_in = 0;
        g.capacity_in = 4;
        g.outputs = nullptr;
        g.size_out = 0;
        g.capacity_out = 4;
    }


    Gate::~Gate() {
        delete [] inputs;
        delete [] outputs;
    }

    void Gate::readStates(std::istream& in) const {
        std::vector<Pin::State> states;
        Pin::State state;

        for (size_t i = 0; i < size_in; ++i) {
            in >> state;
            if (in.fail())
                throw std::invalid_argument("Can not read states of all pins");
            if (state != Pin::State::X)
                throw std::invalid_argument("State of input Pin must be X");
            states.push_back(state);
        }
        for (size_t i = 0; i < size_out; ++i) {
            in >> state;
            if (in.fail())
                throw std::invalid_argument("Can not read states of all pins");
            states.push_back(state);
        }


        for (size_t i = 0; i < size_in; ++i) {
            inputs[i].setState(states[i]);
        }
        for (size_t i = 0; i < size_out; ++i) {
            outputs[i].setState(states[i + size_in]);
        }
    }

    void Gate::addPinIn() {
        if (size_in == capacity_in) {
            sizeDouble(inputs,capacity_in);
        }
        inputs[size_in++] = Pin();
    }


    void Gate::addPinOut() {
        if (size_out == capacity_out) {
            sizeDouble(outputs,capacity_out);
        }
        outputs[size_out++] = Pin(Pin::Type::Output);
    }


    void Gate::addPin(const Pin& pin) {
        if (pin.getType() == Pin::Type::Output) {
            if (size_out == capacity_out) {
                sizeDouble(outputs,capacity_out);
            }
            size_out += 1;
            outputs[size_out - 1] = pin;
        }
        else {
            if (size_in == capacity_in) {
                sizeDouble(inputs, capacity_in);
            }
            size_in += 1;
            inputs[size_in - 1] = pin;
        }
    }

    void Gate::setInputs(const unsigned count_inputs) {
        size_in = 0;

        if (count_inputs > capacity_in) {
            auto* ptr = new Pin[count_inputs * 2];
            delete [] inputs;
            inputs = ptr;
            capacity_in = count_inputs * 2;
        }

        for (size_t i = 0; i < count_inputs; ++i) {
            inputs[i] = Pin();
            size_in += 1;
        }
    }

    void Gate::setOutputs(const unsigned count_outputs) {
        size_out = 0;

        if (count_outputs > capacity_out) {
            auto* ptr = new Pin[count_outputs * 2];
            delete [] outputs;
            outputs = ptr;
            capacity_out = count_outputs * 2;
        }

        for (size_t i = 0; i < count_outputs; ++i) {
            outputs[i] = Pin(Pin::Type::Output);
            size_out += 1;
        }
    }

    Pin& Gate::operator[](const int i) const {
        if (i < -static_cast<int>(size_out) || i > static_cast<int>(size_in) || i == 0)
            throw std::out_of_range("Pin index out of range!");

        if (i > 0)
            return inputs[i - 1];

        return outputs[-i - 1];
    }

    void Gate::removePin(int i) {
        if (i < -static_cast<int>(size_out) || i > static_cast<int>(size_in) || i == 0)
            throw std::out_of_range("Pin index out of range!");

        if (i > 0) {
            inputs[i-1] = inputs[size_in-1];
            --size_in;
        }
        else {
            outputs[-i-1] = outputs[size_out-1];
            --size_out;
        }
    }


    void Gate::operator >>(const Gate& gatein) const {
        constexpr auto ro_pred = [](const auto& pin) { return pin.getConnections() < 3; };
        constexpr auto ri_pred = [](const auto& pin) { return pin.getConnections() == 0; };

        auto ro = std::span(outputs, size_out);
        auto ri = std::span(gatein.inputs, gatein.size_in);

        const auto out = std::ranges::find_if(ro, ro_pred);
        if (out == ro.end()) throw std::logic_error("No pins available for connection");

        const auto in = std::ranges::find_if(ri, ri_pred);
        if (in == ri.end()) throw std::logic_error("No pins available for connection");

        *out >> *in;
    }

    Gate& Gate::operator=(const Gate& g) {
        if (this != &g) {
            auto* new_inputs = new Pin[g.capacity_in];
            auto* new_outputs = new Pin[g.capacity_out];

            delete [] inputs;
            delete [] outputs;

            inputs = new_inputs;
            outputs = new_outputs;

            capacity_in = g.capacity_in;
            capacity_out = g.capacity_out;
            size_in = g.size_in;
            size_out = g.size_out;

            std::copy_n(g.inputs, size_in, inputs);
            std::copy_n(g.outputs, size_out, outputs);
        }
        return *this;
    }

    Gate& Gate::operator=(Gate&& g) noexcept {
        if (this != &g) {
            delete [] inputs;
            delete [] outputs;

            inputs = g.inputs;
            outputs = g.outputs;
            capacity_in = g.capacity_in;
            capacity_out = g.capacity_out;
            size_in = g.size_in;
            size_out = g.size_out;

            g.inputs = nullptr;
            g.outputs = nullptr;
            g.capacity_in = 0;
            g.capacity_out = 0;
            g.size_in = 0;
            g.size_out = 0;
        }

        return *this;
    }

    std::strong_ordering Gate::operator<=>(const Gate& g) const {
        auto cmp = size_in + size_out <=> g.size_in + g.size_out;
        if (cmp != 0)
            return cmp;

        size_t less = 0, more = 0;
        for (auto i = 0; i < size_in; ++i) {
            cmp = inputs[i].getState() <=> g.inputs[i].getState();
            if (cmp < 0) {
                ++less;
            }
            else if (cmp > 0) {
                ++more;
            }
        }

        for (auto i = 0; i < size_out; ++i) {
            cmp = outputs[i].getState() <=> g.inputs[i].getState();
            if (cmp < 0) {
                ++less;
            }
            else if (cmp > 0) {
                ++more;
            }
        }

        if (less > more) {
            return std::strong_ordering::less;
        }
        if (less < more) {
            return std::strong_ordering::greater;
        }
        return std::strong_ordering::equal;
    }

    std::ostream& Gate::printFormat(std::ostream& c) const {
        c << "\t┌─────────────┐\n";
        c << "\t│    Gate     │\n";
        c << "\t├─────────────┤\n";
        unsigned i = 0;
        while (i < this->getNumInputs() && i < this->getNumOutputs()) {
            c << "IN" << i + 1 << " → ";
            c << inputs[i].getState() << "\t\t";

            c << outputs[i].getState();
            c << "→ OUT" << i + 1 << "\n";
            ++i;
        }

        while (i < this->getNumInputs()) {
            c << "IN" << i + 1 << " → ";
            c << inputs[i].getState() << "\t\t  │\n";
            ++i;
        }
        while (i < this->getNumOutputs()) {
            c << "\t│\t\t\t";
            c << outputs[i].getState();
            c << "→ OUT" << i + 1 << "\n";
            ++i;
        }

        c << "\t└─────────────┘\n";

        return c;
    }
}


std::ostream& operator <<(std::ostream& c, const units::Gate& g) {
    c << "Inputs:\n";
    const units::Pin* inputs = g.getInputs();
    for (size_t i = 0; i < g.getNumInputs(); ++i) {
        c << inputs[i];
    }

    c << "Outputs:\n";
    const units::Pin* outputs = g.getOutputs();
    for (size_t i = 0; i < g.getNumOutputs(); ++i) {
        c << outputs[i];
    }

    return c;
}


std::istream& operator >>(std::istream& c, units::Gate& g) {
    units::Gate w;
    unsigned count_inputs, count_outputs;
    c >> count_inputs;
    c >> count_outputs;
    w.setInputs(count_inputs);
    w.setOutputs(count_outputs);

    try {
        w.readStates(c);
    }
    catch (const std::logic_error&) {
        c.setstate(std::ios::failbit);
        return c;
    }

    g = std::move(w);

    return c;
}
