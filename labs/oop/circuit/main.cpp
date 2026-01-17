#include <iostream>

#include "gate.h"
#include "dialog.h"

using namespace units;
int main() {

    // std::vector<Pin::State> in_states = {Pin::State::X, Pin::State::X, Pin::State::X, Pin::State::X};
    // std::vector<Pin::State> out_states = {Pin::State::High, Pin::State::X, Pin::State::Low, Pin::State::High};
    // const Gate G(in_states, out_states);
    // const Pin *outputs = G.getOutputs();
    // for (size_t j = 0; j < G.getNumOutputs(); ++j) {
    //     std::cout << outputs[j].getType() << std::endl;
    //     std::cout << outputs[j].getState() << std::endl;
    // }

    //std::vector<units::Pin> pins;
    //std::vector<units::Gate> gates;
    //dialog(pins, gates);

    //Gate g;
    //std::cin >> g;

    /*Gate g(14, 16);                                         //initializing constructor
    g.printFormat(std::cout);
    */

    // Gate G(1, 0);
    // Gate F(0, 1);
    // F >> G;


    Gate G;
    Gate F;
    F.addPinOut();
    G.addPinIn();
    F >> G;
    std::cout << G << "\n" << std::endl;
    G.addPinIn();
    G.addPinIn();
    G.addPinIn();
    G.addPinIn();
    std::cout << G;

    // Pin P;
    // Pin R(std::move(P));

    return 0;
}
