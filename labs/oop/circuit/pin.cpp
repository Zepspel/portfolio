//
// Created by pavel on 29.11.2025.
//

#include <vector>
#include <algorithm>
#include <ostream>
#include <iostream>
#include <stdexcept>
#include "pin.h"

namespace units {
     Pin::Pin(Type type, State state) //initializing constructor
    {
        if (type == Type::Input && state != State::X)
            throw std::invalid_argument("The input pin without connections must be X");

        this->type = type;
        this->state = state;
    }


    Pin::Pin(Pin&& other) noexcept : type(other.type), state(other.state), joined(std::move(other.joined)) {

        for (auto* p : joined) {
            if (p)
                std::ranges::replace(p->joined, &other, this);
        }
    }


    Pin::~Pin() {
        for (auto* p : joined) {
            std::erase(p->joined, this);
        }
    }

    void Pin::setState(State state) {
        if (type == Type::Input && joined.empty() && state != State::X)
            throw std::invalid_argument("The input terminal without connections must be X");
        if (type == Type::Input && !joined.empty())
            throw std::logic_error("Input pin has connection, can not change signal");

        if (type == Type::Output && !joined.empty()) {
            for (auto* input : joined) {
                input->state = state;
            }
        }
        this->state = state;
    }


    void Pin::setType(const Type type) {
        if (!joined.empty())
            throw std::logic_error("Can not change type of pin with connections");

        this->type = type;
    }


    Pin Pin::operator ++(int) {
        if (state == State::X)
            return *this;
        if (state == State::High)
            throw std::logic_error("State is high already");

        Pin old = *this;

        setState(State::High);
        setState(State::Low);


        return old;
    }

    Pin& Pin::operator ++() {
        if (state == State::X)
            return *this;
        if (state == State::High)
            throw std::logic_error("State is high already");



        setState(State::High);
        setState(State::Low);

        return *this;
    }

    Pin Pin::operator --(int) {
        if (state == State::X)
            return *this;
        if (state == State::Low)
            throw std::logic_error("State is low already");

        Pin old = *this;

        setState(State::Low);
        setState(State::High);


        return old;
    }

    Pin& Pin::operator --() {
        if (state == State::X)
            return *this;
        if (state == State::Low)
            throw std::logic_error("State is low already");

        setState(State::Low);
        setState(State::High);

        return *this;
    }

    bool Pin::operator!() {

         if (state == State::Low) {
             setState(State::High);
             return true;
         }
         if (state == State::High) {
             setState(State::Low);
             return true;
         }
         return false;

    }

    Pin& Pin::operator=(const Pin& p) {
        this->type = p.type;
        this->state = p.state;
        return *this;
    }

    Pin& Pin::operator=(Pin&& other) noexcept {
        this->type = other.type;
        this->state = other.state;

        for (auto *p: joined) {
            if (p) {
                std::erase(p->joined, this);
            }
        }

        joined = std::move(other.joined);
        other.joined.clear();


        for (auto* p : joined) {
            if (p)
                std::ranges::replace(p->joined, &other, this);
        }

        return *this;
    }


    void Pin::operator>>(Pin& input) {
        if (type == Type::Input || input.type == Type::Output)
            throw std::invalid_argument(R"(Operator ">>" works like "output >> input")");
        if (joined.size() == 3 || input.joined.size() == 1)
            throw std::logic_error("Max number of connection already exists in input pin");

        input.state = state;
        joined.push_back(&input);
        input.joined.push_back(this);
    }

    void Pin::disconnect(Pin& p) {
        const auto iter = std::ranges::find(joined, &p);
        if (iter == joined.end())
            throw std::logic_error("There is no connection");


        joined.erase(iter);
        std::erase(p.joined, this);
    }

}


std::ostream& operator <<(std::ostream& c, const units::Pin::Type& t) {
    if (t == units::Pin::Type::Input)
        c << "Input ";
    else
        c << "Output ";
    return c;
}

std::ostream& operator <<(std::ostream& c, const units::Pin::State& s) {
    if (s == units::Pin::State::High)
        c << "High ";
    else if (s == units::Pin::State::Low)
        c << "Low ";
    else
        c << "X ";
    return c;
}

std::ostream& operator <<(std::ostream& c, const units::Pin& p) {
    c << p.getType() << p.getState() << p.getConnections() << std::endl;
    return c;
}

std::istream& operator >>(std::istream& c, units::Pin::State& s) {
    std::string str;
    c >> str;
    if (str == "High" || str == "high" || str == "1")
        s = units::Pin::State::High;
    else if (str == "Low" || str == "low" || str == "0")
        s = units::Pin::State::Low;
    else if (str == "X" || str == "x")
        s = units::Pin::State::X;
    else
        c.setstate(std::ios::failbit);
    return c;
}

std::istream& operator >>(std::istream& c, units::Pin::Type& t) {
    std::string str;
    c >> str;
    if (str == "Input" || str == "input")
        t = units::Pin::Type::Input;
    else if (str == "Output" || str == "output")
        t = units::Pin::Type::Output;
    else
        c.setstate(std::ios::failbit);
    return c;
}

std::istream& operator >>(std::istream& c, units::Pin& p) {
    units::Pin::Type type;
    units::Pin::State state;

    c >> type;
    c >> state;

    if (c.fail())
        return c;

    p.setType(type);
    p.setState(state);

    return c;
}
