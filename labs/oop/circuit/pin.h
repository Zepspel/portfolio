//
// Created by pavel on 29.11.2025.
//

#ifndef INC_2_PIN_H
#define INC_2_PIN_H

namespace units {
    /**
     * @brief Class representing pin
     * @details Pin has attribute: 1) Type (input/output), 2) State (high, low, x),
     * 3) joined - array of pointers to other pins with which it's connected
     */
    class Pin {
    public:
        enum class Type : unsigned char { Input, Output };
        enum class State : unsigned char { Low, X, High };
    private:
        Type type;
        State state;
        std::vector<Pin*> joined;

    public:
        /**
         * @brief void constructor
         */
        Pin(): type(Type::Input), state(State::X) {}
        /**
         * @brief constructor with certain type of pin
         * @param type pin type
         */
        explicit Pin(Type type): type(type), state(State::X) {}


        /**
         * @brief full constructor
         * @param type input/output
         * @param state high/low/x
         * @throw std::invalid_argument if input pin has more than 1 connection
         * @throw std::invalid_argument if output pin has more than 3 connections
         * @throw std::invalid_argument if input pin without connection don't have X state
         */
        Pin(Type type, State state);


        /**
         * @brief copying constructor
         * @details does not copy joined pins
         * @param other pin to copying
         */
        Pin(const Pin& other) : type(other.type), state(other.state) {}

        /**
         * @brief moving constructor
         * @details also move joined pins
         * @param other pin to moving
         */
        Pin(Pin&& other) noexcept;

        /**
        * @brief destructor disconnect connected pins with deleted one
        */
        ~Pin();

        /**
         * @brief getter for type of pin
         * @return pin type
         */
        [[nodiscard]] Type getType() const { return type; }
        /**
         * @brief getter for number of connection
         * @return connection number
         */
        [[nodiscard]] unsigned getConnections() const { return joined.size(); }
        /**
         * @brief getter for state of pin
         * @return pin state
         */
        [[nodiscard]] State getState() const { return state; }


        /**
         * @breif setter of pin state
         * @param state high/low/x
         * @throw std::invalid_argument if you try to assign state different from X
         * for input pin without connections
         * @throw std::logic_error if it's already connected input pin
         */
        void setState(State state);


        /**
         * @brief setter of pin type
         * @param type input/output
         * @throw std::logic_error if you're changing type of pin which one has connection
         */
        void setType(Type type);
        //void set(Type type, unsigned connections, State state);

        /**
         * @brief changes state like 0->1->0
         * @return Pin
         * @throw std::out_of_range if max number of connections has been reached
         */
        Pin operator ++(int);
        Pin &operator ++();

        /**
         * @brief changes state like 1->0->1
         * @return Pin
         * @throw std::out_of_range if The min number of connections has been reached
         */
        Pin operator --(int);
        Pin &operator --();

        /**
         * @brief changes state to opposite
         * @details doesn't change X state
         * @return true if state was changed else false
         */
        bool operator !();

        /**
         * @brief copying operator
         * @details does not copy joined pins
         * @param p pin for copying
         * @return p
         */
        Pin& operator=(const Pin& p);

        /**
         * @brief moving operator
         * @details does not move joined pins
         * @param other pin for moving
         * @return p
         */
        Pin& operator=(Pin&& other) noexcept;

        /**
         * @brief connects output pin with input pin
         * @details meanwhile output pin state copied to input pin state,
         * that occurs during whole connection time, it can be broken by disconnection method
         * @param input pin with input type
         * @throw std::invalid_argument if it doesn't match the template "output >> input"
         * @throw std::logic_error if max number of connection has been reached
         */
        void operator >> (Pin& input);

        /**
         * @brief disconnects output pin with input pin
         * @details stops the action of connection operator ">>"
         * @param p pin with input type
         * @throw std::logic_error if there is no connection
         */
        void disconnect(Pin& p);
    };
}

std::ostream & operator << (std::ostream &c, const units::Pin::Type &);
std::ostream & operator << (std::ostream &c, const units::Pin::State &);
/**
 * @brief prints attributes of pin (Type, State, Number of connections)
 * @param c output stream
 * @return c
 */
std::ostream & operator << (std::ostream &c, const units::Pin &);


std::istream & operator >> (std::istream &c, units::Pin::State &);
std::istream & operator >> (std::istream &c, units::Pin::Type &);
/**
 * @brief read attributes of pin (Type, State, Number of connections)
 * @param c input stream
 * @return c
 * @throw std::logic_error if it can't read all attributes (because of EOF for example)
 */
std::istream & operator >> (std::istream &c, units::Pin &);

#endif //INC_2_PIN_H