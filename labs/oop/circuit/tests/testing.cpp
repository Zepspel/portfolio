#define CATCH_CONFIG_MAIN

#include <catch2/catch_all.hpp>
#include "../gate.h"
#include "../pin.h"


using namespace units;

TEST_CASE("Pin class testing") {
    SECTION("Pin constructors") {
        SECTION("Default constructor") {
            const Pin p;
            REQUIRE(p.getType() == Pin::Type::Input);
            REQUIRE(p.getConnections() == 0);
            REQUIRE(p.getState() == Pin::State::X);
        }
        SECTION("Pin copy constructor") {
            Pin p1(Pin::Type::Output, Pin::State::High);

            Pin p2(p1);

            REQUIRE(p2.getType() == Pin::Type::Output);
            REQUIRE(p2.getConnections() == 0);
            REQUIRE(p2.getState() == Pin::State::High);
        }
        SECTION("Pin move constructor") {
            Pin p1(Pin::Type::Output, Pin::State::High);

            Pin p2(std::move(p1));

            REQUIRE(p2.getType() == Pin::Type::Output);
            REQUIRE(p2.getConnections() == 0);
            REQUIRE(p2.getState() == Pin::State::High);

            SECTION("Deal with connections") {
                Pin P1(Pin::Type::Input);
                Pin P2(Pin::Type::Output, Pin::State::Low);
                P2 >> P1;
                Pin P3(std::move(P2));
                REQUIRE(P3.getConnections() == 1);
            }
        }
        SECTION("Constructor with type") {
            Pin p(Pin::Type::Input);
            REQUIRE(p.getType() == Pin::Type::Input);
            Pin p2(Pin::Type::Output);
            REQUIRE(p2.getType() == Pin::Type::Output);
        }
        SECTION("Constructor with 2 parameters") {
            Pin p(Pin::Type::Input, Pin::State::X);
            REQUIRE(p.getType() == Pin::Type::Input);
            REQUIRE(p.getConnections() == 0);
            REQUIRE(p.getState() == Pin::State::X);


            Pin p2(Pin::Type::Output, Pin::State::High);
            REQUIRE(p2.getType() == Pin::Type::Output);
            REQUIRE(p2.getConnections() == 0);
            REQUIRE(p2.getState() == Pin::State::High);
        }
        SECTION("Throws test") {
            SECTION("Input pin without connections must has X state") {
                REQUIRE_THROWS_AS(Pin(Pin::Type::Input, Pin::State::High), std::logic_error);
            }

        }

    }
    SECTION("Pin setters") {
        SECTION("Type setter") {
            Pin A;
            A.setType(Pin::Type::Output);
            REQUIRE(A.getType() == Pin::Type::Output);
            A.setType(Pin::Type::Input);
            REQUIRE(A.getType() == Pin::Type::Input);
        }
        SECTION("State setter") {
            Pin A(Pin::Type::Output);
            A.setState(Pin::State::High);
            REQUIRE(A.getState() == Pin::State::High);
            A.setState(Pin::State::Low);
            REQUIRE(A.getState() == Pin::State::Low);
        }
        SECTION("Throws test") {
            SECTION("Type setter") {
                Pin A(Pin::Type::Output, Pin::State::High);
                Pin B;
                A >> B;
                REQUIRE_THROWS_AS(A.setType(Pin::Type::Input), std::logic_error);
            }
            SECTION("State setter") {
                SECTION("Input pin with connection") {
                    Pin A(Pin::Type::Input, Pin::State::X);
                    Pin B(Pin::Type::Output);
                    B >> A;
                    REQUIRE_THROWS_AS(A.setState(Pin::State::Low), std::logic_error);
                }
                SECTION("Input pin without connection must has X state") {
                    Pin A;
                    REQUIRE_THROWS_AS(A.setState(Pin::State::Low), std::logic_error);
                }
            }
        }

    }
    SECTION("Operators test") {
        SECTION("copy = ") {
            const Pin p1(Pin::Type::Output, Pin::State::High);

            Pin p2 = p1;

            REQUIRE(p2.getType() == Pin::Type::Output);
            REQUIRE(p2.getConnections() == 0);
            REQUIRE(p2.getState() == Pin::State::High);
        }
        SECTION ("move = ") {
            Pin p1(Pin::Type::Output, Pin::State::High);

            Pin p2 = std::move(p1);

            REQUIRE(p2.getType() == Pin::Type::Output);
            REQUIRE(p2.getConnections() == 0);
            REQUIRE(p2.getState() == Pin::State::High);
        }
        SECTION("++") {
            SECTION("Prefix") {
                SECTION("Input pin") {
                    Pin A;
                    Pin B(Pin::Type::Output, Pin::State::High);
                    B >> A;
                    REQUIRE_THROWS_AS(++A, std::logic_error);
                }
                SECTION("Output pin") {
                    Pin A(Pin::Type::Output, Pin::State::Low);
                    ++A;
                    REQUIRE(A.getState() == Pin::State::Low);
                    A.setState(Pin::State::High);
                    REQUIRE_THROWS_AS(++A, std::logic_error);
                }
            }
            SECTION("Postfix") {
                SECTION("Input pin") {
                    Pin A;
                    Pin B(Pin::Type::Output, Pin::State::High);
                    B >> A;
                    REQUIRE_THROWS_AS(A++, std::logic_error);
                }
                SECTION("Output pin") {
                    Pin A(Pin::Type::Output, Pin::State::Low);
                    A++;
                    REQUIRE(A.getState() == Pin::State::Low);
                    A.setState(Pin::State::High);
                    REQUIRE_THROWS_AS(A++, std::logic_error);
                }
            }
        }
        SECTION("--") {
            SECTION("Prefix") {
                Pin A;
                Pin B(Pin::Type::Output, Pin::State::High);
                B >> A;
                REQUIRE_THROWS_AS(--A, std::logic_error);
            }
            SECTION("Postfix") {
                Pin A(Pin::Type::Output, Pin::State::High);
                A--;
                REQUIRE(A.getState() == Pin::State::High);
                A.setState(Pin::State::Low);
                REQUIRE_THROWS_AS(A--, std::logic_error);
            }
        }
        SECTION("Operator !") {
            Pin A(Pin::Type::Output, Pin::State::High);
            REQUIRE(!A == true);
            REQUIRE(A.getState() == Pin::State::Low);
            Pin B(Pin::Type::Output, Pin::State::X);
            REQUIRE(!B == false);
            Pin C;
            A >> C;
            REQUIRE_THROWS_AS(!C, std::logic_error);
        }
        SECTION("Connection operator >>") {
            SECTION("output >> input") {
                Pin A(Pin::Type::Output, Pin::State::High);
                Pin B(Pin::Type::Input, Pin::State::X);
                A >> B;
                REQUIRE(B.getState() == Pin::State::High);
                REQUIRE(A.getConnections() == 1);
                REQUIRE(B.getConnections() == 1);

                A.setState(Pin::State::Low);
                REQUIRE(B.getState() == Pin::State::Low);

                REQUIRE_THROWS_AS(A >> B, std::logic_error);                    //already connected

                SECTION("Max number of connection") {
                    Pin P(Pin::Type::Output, Pin::State::High);
                    Pin W(Pin::Type::Input, Pin::State::X);

                    Pin R(Pin::Type::Output, Pin::State::Low);
                    R >> W;
                    REQUIRE_THROWS_AS(P >> W, std::logic_error);
                }
            }

            SECTION("input >> output") {
                Pin C(Pin::Type::Output, Pin::State::High);
                Pin D(Pin::Type::Input, Pin::State::X);
                REQUIRE_THROWS_AS(D >> C, std::logic_error);
            }
        }
    }
    SECTION("Disconnection method") {
        SECTION("Not connected") {
            Pin A(Pin::Type::Output, Pin::State::High);
            Pin B(Pin::Type::Input, Pin::State::X);
            REQUIRE_THROWS_AS(A.disconnect(B), std::logic_error);
        }
        SECTION("output.disconnection(input)") {
            Pin A(Pin::Type::Output, Pin::State::High);
            Pin B(Pin::Type::Input, Pin::State::X);
            A >> B;
            A.disconnect(B);
            A.setState(Pin::State::Low);
            REQUIRE(B.getState() == Pin::State::High);
            REQUIRE(A.getConnections() == 0);
            REQUIRE(B.getConnections() == 0);
        }
    }
}



TEST_CASE("Gate class testing") {
    SECTION("Gate constructors") {
        SECTION("Constructor with number of input and output pin") {
            const unsigned i = GENERATE(take(10, random(0, 100)));
            const unsigned o = GENERATE(take(10, random(0, 100)));
            Gate G(i, o);
            REQUIRE(G.getNumInputs() == i);
            REQUIRE(G.getNumOutputs() == o);
        }
        SECTION("Copying constructor") {
            std::vector<Pin::State> in_states = {Pin::State::X, Pin::State::X, Pin::State::X, Pin::State::X};
            std::vector<Pin::State> out_states = {Pin::State::High, Pin::State::X, Pin::State::Low, Pin::State::High};
            const Gate G2(in_states, out_states);
            Gate G(G2);
            const Pin *inputs = G.getInputs();
            for (size_t i = 0; i < G.getNumInputs(); ++i) {
                REQUIRE(inputs[i].getType() == Pin::Type::Input);
                REQUIRE(inputs[i].getState() == in_states[i]);
            }

            const Pin *outputs = G.getOutputs();
            for (size_t j = 0; j < G.getNumOutputs(); ++j) {
                REQUIRE(outputs[j].getType() == Pin::Type::Output);
                REQUIRE(outputs[j].getState() == out_states[j]);
            }
        }
        SECTION("Moving constructor") {
            std::vector<Pin::State> in_states = {Pin::State::X, Pin::State::X, Pin::State::X, Pin::State::X};
            std::vector<Pin::State> out_states = {Pin::State::High, Pin::State::X, Pin::State::Low, Pin::State::High};
            Gate G2(in_states, out_states);
            Gate G(std::move(G2));
            const Pin *inputs = G.getInputs();
            for (size_t i = 0; i < G.getNumInputs(); ++i) {
                REQUIRE(inputs[i].getType() == Pin::Type::Input);
                REQUIRE(inputs[i].getState() == in_states[i]);
            }

            const Pin *outputs = G.getOutputs();
            for (size_t j = 0; j < G.getNumOutputs(); ++j) {
                REQUIRE(outputs[j].getType() == Pin::Type::Output);
                REQUIRE(outputs[j].getState() == out_states[j]);
            }

            REQUIRE(G2.getNumInputs() == 0);
            REQUIRE(G2.getNumOutputs() == 0);
        }
        SECTION("Constructor with states vector") {
            std::vector<Pin::State> in_states = {Pin::State::X, Pin::State::X, Pin::State::X, Pin::State::X};
            std::vector<Pin::State> out_states = {Pin::State::High, Pin::State::X, Pin::State::Low, Pin::State::High};
            const Gate G(in_states, out_states);
            const Pin *inputs = G.getInputs();
            for (size_t i = 0; i < G.getNumInputs(); ++i) {
                REQUIRE(inputs[i].getType() == Pin::Type::Input);
                REQUIRE(inputs[i].getState() == in_states[i]);
            }

            const Pin *outputs = G.getOutputs();
            for (size_t j = 0; j < G.getNumOutputs(); ++j) {
                REQUIRE(outputs[j].getType() == Pin::Type::Output);
                REQUIRE(outputs[j].getState() == out_states[j]);
            }
        }
    }
    SECTION("Add pin") {
        SECTION("Without parameters") {
            Gate G;
            G.addPinIn();
            REQUIRE(G.getNumInputs() == 1);
            G.addPinIn();
            REQUIRE(G.getNumInputs() == 2);

            G.addPinOut();
            REQUIRE(G.getNumOutputs() == 1);
            G.addPinOut();
            REQUIRE(G.getNumOutputs() == 2);
            SECTION("doubling the capacity") {
                Gate G2;
                G2.addPinIn();
                G2.addPinIn();
                G2.addPinIn();
                G2.addPinIn();
                G2.addPinIn();
                REQUIRE(G2.getNumInputs() == 5);
                G2.addPinOut();
                G2.addPinOut();
                G2.addPinOut();
                G2.addPinOut();
                G2.addPinOut();
                REQUIRE(G2.getNumOutputs() == 5);
            }
        }
        SECTION("With certain pin") {
            Gate G;
            Pin P(Pin::Type::Output, Pin::State::High);
            G.addPin(P);
            REQUIRE(G.getNumOutputs() == 1);
            REQUIRE(G[-1].getType() == Pin::Type::Output);
            REQUIRE(G[-1].getState() == Pin::State::High);
            REQUIRE(G[-1].getConnections() == 0);
        }
    }
    SECTION("doubleSize") {
        Gate G;
        Gate F;
        F.addPinOut();
        G.addPinIn();
        F >> G;
        G.addPinIn();
        G.addPinIn();
        G.addPinIn();
        G.addPinIn();

        std::stringstream buf;
        buf << G;
        REQUIRE(buf.str() ==    "Inputs:\n"
                                "Input X 1\n"
                                "Input X 0\n"
                                "Input X 0\n"
                                "Input X 0\n"
                                "Input X 0\n"
                                "Outputs:\n");

        REQUIRE(G[1].getConnections() == 1);
    }
    SECTION("Delete pin") {
        Gate G(4, 3);
        G.removePin(2);
        G.removePin(-3);
        REQUIRE(G.getNumInputs() == 3);
        REQUIRE(G.getNumOutputs() == 2);

    }
    SECTION("Setters test") {
        SECTION("Inputs") {
            Gate G;
            const int i = GENERATE(take(10, random(0, 50)));
            G.setInputs(i);
            REQUIRE(G.getNumInputs() == i);
        }
        SECTION("Outputs") {
            Gate G;
            const int i = GENERATE(take(10, random(0, 50)));
            G.setOutputs(i);
            REQUIRE(G.getNumOutputs() == i);
        }
        SECTION("Doubling the capacity") {
            Gate G;
            G.setInputs(5);
            REQUIRE(G.getNumInputs() == 5);
            G.setOutputs(5);
            REQUIRE(G.getNumOutputs() == 5);
        }
    }
    SECTION("Operators test") {
        SECTION("Operator <=> ") {
            std::vector<Pin::State> in_states = {Pin::State::X, Pin::State::X, Pin::State::X, Pin::State::X};
            std::vector<Pin::State> out_states = {Pin::State::High, Pin::State::X, Pin::State::Low, Pin::State::High};
            const Gate G(in_states, out_states);

            std::vector<Pin::State> in_states2 = {Pin::State::X, Pin::State::X, Pin::State::X, Pin::State::X};
            std::vector<Pin::State> out_states2 = {Pin::State::Low, Pin::State::X, Pin::State::Low, Pin::State::High};
            const Gate G2(in_states2, out_states2);

            auto cmp = G <=> G2;
            REQUIRE(cmp == std::strong_ordering::greater);
            //REQUIRE(static_cast<int>(cmp) > 0);

        }
        SECTION("Operator []") {
            SECTION("In pin") {
                SECTION("Out of range") {
                    Gate G;
                    G.setInputs(100);
                    const int i = GENERATE(take(100, random(101, 1000)));
                    REQUIRE_THROWS_AS(G[i], std::out_of_range);
                }
                SECTION("Normal work") {
                    Gate G;
                    G.setInputs(100);
                    const int i = GENERATE(take(100, random(1, 100)));
                    REQUIRE(G[i].getType() == Pin::Type::Input);
                }
            }
            SECTION("Out pin") {
                SECTION("Out of range") {
                    Gate G;
                    G.setOutputs(100);
                    const int i = GENERATE(take(100, random(101, 1000)));
                    REQUIRE_THROWS_AS(G[-i], std::out_of_range);
                }
                SECTION("Normal work") {
                    Gate G;
                    G.setOutputs(100);
                    const int i = GENERATE(take(100, random(1, 100)));
                    CAPTURE(i);
                    REQUIRE(G[-i].getType() == Pin::Type::Output);
                }
            }

        }
        SECTION("Connection operator >>") {
            SECTION("output >> input") {
                Gate G;
                G.addPin(Pin(Pin::Type::Output, Pin::State::High));
                Gate F;
                F.addPin(Pin(Pin::Type::Input, Pin::State::X));

                G >> F;
                REQUIRE(G[-1].getConnections() == 1);

                Gate R;
                R.addPin(Pin(Pin::Type::Input, Pin::State::X));

                G >> R;
                REQUIRE(G[-1].getConnections() == 2);

                REQUIRE(F[1].getState() == Pin::State::High);
                REQUIRE(R[1].getState() == Pin::State::High);
                G[-1].setState(Pin::State::Low);
                REQUIRE(F[1].getState() == Pin::State::Low);
                REQUIRE(R[1].getState() == Pin::State::Low);
            }
            SECTION("No pins available for connection") {
                Gate G;
                G.addPin(Pin(Pin::Type::Output, Pin::State::High));
                Gate F;
                F.addPin(Pin(Pin::Type::Input, Pin::State::X));
                Gate W;
                W.addPin(Pin(Pin::Type::Output, Pin::State::High));
                W >> F;
                REQUIRE_THROWS_AS(G >> F, std::logic_error);
            }
        }
    }
    SECTION("Read from a stream") {
        SECTION("Read states") {
            std::istringstream input("x X 0 1");
            Gate G(1, 3);
            G.readStates(input);
            REQUIRE(G.getInputs()[0].getState() == Pin::State::X);
            REQUIRE(G.getOutputs()[0].getState() == Pin::State::X);
            REQUIRE(G.getOutputs()[1].getState() == Pin::State::Low);
            REQUIRE(G.getOutputs()[2].getState() == Pin::State::High);
            std::istringstream input2("a");
            REQUIRE_THROWS_AS(G.readStates(input2), std::invalid_argument);
            Gate K(0, 1);
            std::istringstream input3("b");
            REQUIRE_THROWS_AS(K.readStates(input3), std::invalid_argument);
        }
    }
}

TEST_CASE("Input testing") {
    SECTION("Input state") {
        std::istringstream input("x 0 1 a");
        Pin::State A, B, C;
        input >> A >> B >>C;
        REQUIRE(A == Pin::State::X);
        REQUIRE(B == Pin::State::Low);
        REQUIRE(C == Pin::State::High);
        input >> B;
        REQUIRE(input.fail());
    }
    SECTION("Input type") {
        std::istringstream input("Input output zzz");
        Pin::Type A, B;
        input >> A >> B;
        REQUIRE(A == Pin::Type::Input);
        REQUIRE(B == Pin::Type::Output);
        input >> A;
        REQUIRE(input.fail());
    }
    SECTION("Input pin") {
        std::istringstream input("Output High ZZZ");
        Pin P;
        input >> P;
        REQUIRE(P.getState() == Pin::State::High);
        REQUIRE(P.getType() == Pin::Type::Output);
        REQUIRE(P.getConnections() == 0);
        input >> P;
        REQUIRE(input.fail());
    }
    SECTION("Input gate") {
        std::istringstream input("2 3 x X 1 0 x");
        Gate G;
        input >> G;
        REQUIRE(G[1].getState() == Pin::State::X);
        REQUIRE(G[-1].getState() == Pin::State::High);
        REQUIRE(G[-2].getState() == Pin::State::Low);
    }
}

TEST_CASE("Output testing") {
    std::stringstream buf;
    SECTION("Output pin type") {
        auto T = Pin::Type::Output;
        buf << T;
        REQUIRE(buf.str() == "Output ");
        buf.str("");

        T = Pin::Type::Input;
        buf << T;
        REQUIRE(buf.str() == "Input ");
        buf.str("");
    }
    SECTION("Output pin state") {
        auto S = Pin::State::High;
        buf << S;
        REQUIRE(buf.str() == "High ");
        buf.str("");

        S = Pin::State::Low;
        buf << S;
        REQUIRE(buf.str() == "Low ");
        buf.str("");

        S = Pin::State::X;
        buf << S;
        REQUIRE(buf.str() == "X ");
        buf.str("");
    }
    SECTION("Output pin") {
        Pin P(Pin::Type::Output, Pin::State::High);
        buf << P;
        REQUIRE(buf.str() == "Output High 0\n");
    }
}







