//
// Created by pavel on 01.10.2025.
//

#ifndef INC_2_GATE_H
#define INC_2_GATE_H
#include <stdexcept>
#include <vector>
#include "pin.h"

namespace units {

    /**
     * @brief Class representing gate which are set of input and output pins
     * @details Gate has array of input and output pins and its size\n
     * also capacity for each array (user can't look at current capacity even through getters)\n
     * it simply doubles when size value reach capacity value
     */
    class Gate {
    private:
        Pin *inputs;
        size_t size_in;
        size_t capacity_in;

        Pin *outputs;
        size_t size_out;
        size_t capacity_out;

        /**
         * doubles capacity of array
         * @param arr array
         * @param capacity size of array
         */
        static void sizeDouble(Pin*& arr, size_t &capacity);

    public:

        /**
        * @brief default constructor
        * @details set capacity in and out = 4
        * @throw std::bad_alloc if it can't allocate memory
        */
        Gate();

        /**
         * @brief constructor
         * @param count_inputs number of input pin in pin description
         * @param count_outputs number of output pin in pin description
         * @throw std::bad_alloc if it can't allocate memory for input and output array
         */
        Gate(unsigned count_inputs, unsigned count_outputs);


        /**
         * @brief constructor using description of pins
         * @param in_states vector of states for in pins
         * @param out_states vector of states for out pins
         * @throw std::bad_alloc if it can't allocate memory for input and output array
         */
        Gate(const std::vector<Pin::State>& in_states, const std::vector<Pin::State>& out_states);
        /**
         * @brief copying constructor
         * @param g gate object
         */
        Gate(const Gate &g);

        /**
         * @brief moving constructor
         * @param g gate object
         */
        Gate(Gate &&g) noexcept;
        /**
         * @brief destructor cleans memory which wad used by in and out array
         */
        ~Gate();

        /**
         * @brief getter for the 2 field of class
         * @return size of input pins array
         */
        [[nodiscard]] unsigned getNumInputs() const { return size_in; }

        /**
         * @brief getter for the 5 field of class
         * @return size of output pins array
         */
        [[nodiscard]] unsigned getNumOutputs() const { return size_out; }

        /**
         * @brief getter for the 1 field of class
         * @return inputs array like const pointer
         */
        [[nodiscard]] const Pin* getInputs() const { return inputs; }

        /**
         * @brief getter for the 4 field of class
         * @return outputs array like const pointer
         */
        [[nodiscard]] const Pin* getOutputs() const { return outputs; }


        /**
         * @brief read pin states (High, Low, X) for all pins that are used by this gate
         * @param in The input stream
         * @throw std::invalid_argument if state was invalid
         * (not x for input pin or some strange word for output pin)
         * @throw std::logic_error if it can't read state of some pin
         */
        void readStates(std::istream& in) const;



        /**
         * @brief add one input pin with X state, 0 connections
         * @details may double capacity of input array if size_in == capacity_in
         * @throw std::bad_alloc if it can't double capacity
         */
        void addPinIn();

        /**
       * @brief add one output pin with X state, 0 connections
       * @details may double capacity of output array if size_out == capacity_out
       * @throw std::bad_alloc if it can't double capacity
       */
        void addPinOut();

        /**
         * @brief add already existing pin
         * @param pin
         * @details may double capacity of array if size == capacity
         * @throw std::bad_alloc if it can't double capacity
         */
        void addPin(const Pin& pin);


        /**
         * @brief fill input pins array with default pins
         * @details may double capacity of input array if size_in == capacity_in,\n
         * erases the pins that were used!!!
         * @param count_inputs number of input pins
         * @throw std::bad_alloc if it can't double capacity
         */
        void setInputs(unsigned count_inputs);

        /**
         * @brief fill output pins array with default pins
         * @details may double capacity of input array if size_out == capacity_out,\n
         * erases the pins that were used!!!
         * @param count_outputs number of output pins
         * @throw std::bad_alloc if it can't double capacity
         */
        void setOutputs(unsigned count_outputs);


        /**
         * @brief get input or output pin
         * @details [i] "i" input pin, [-i] "i" output pin\n
         * indexing start with 1, not 0!!
         * @param i number of pin
         * @return pin
         */
        Pin& operator[](int i) const;

        void removePin(int i);

        /**
         * @brief connects output pin first gate with input pin second gate
         * @details it searches first pins available for connection
         * @param gatein
         * @throw std::logic_error if there are no pins available for connection
         */
        void operator>>(const Gate& gatein) const;

        /**
         * @brief copying operator '=' for gate
         * @param g gate object
         * @return link to the gate
         */
        Gate& operator=(const Gate& g);

        /**
         * moving operator '=' for gate
         * @param g gate object
         * @return link to the gate
         */
        Gate& operator=(Gate&& g) noexcept;

        /**
         *@brief spaceship operator
         *@details compares number of pins
         *If it's equal then compares states of all pin
         * @param g gate object
         * @return std::strong_ordering::equal if number of pins equal and sum of states too
         *std::strong_ordering::less if number of pins less then g has or number of pins equal and sum of states less than g has
         *std::strong_ordering::greater if number of pins greater than g has or number of pins equal and sum of states greater then g has
         */
        std::strong_ordering operator<=>(const Gate& g) const;


        /**
         * @brief prints gate in pseudo graphic format
         * @param c output stream
         * @return c
         */
        std::ostream & printFormat(std::ostream &c) const;

    };

} // units


/**
 * @brief prints list of input and output pins using operator "<<" for everyone
 * @param c output stream
 * @return c
 */
std::ostream & operator << (std::ostream &c, const units::Gate &);


/**
 * @brief read all attributes for gate
 * @details overwrite current attributes
 * @param c input stream
 * @return c
 *
 */
std::istream & operator >> (std::istream &c, units::Gate &);


#endif //INC_2_GATE_H