#ifndef LODGE_ENCODER_H
#define LODGE_ENCODER_H

#include <vector>

using namespace std;
namespace lodge {

    class encoder {
    private:
        encoder() = default;
    };

    template<class T>
    class lsb : public encoder {

    private:
        lsb() = default;

    public:
        static void flatten_bit(T &input);

        static void write_lsb(T &input, T replacement);

        static T read_lsb(T &input);

        static void write_lsb_array(T *input, T *replacement);

        static std::vector<T> read_lsb_array(T *input);

        static void set_bit(T &input, T replacement, unsigned long n);
    };

    template<class T>
    void lodge::lsb<T>::flatten_bit(T &input) {
        input &= ~(1UL << 0UL);
    }

    template<class T>
    void lodge::lsb<T>::write_lsb(T &input, T replacement) {
        lodge::lsb<T>::flatten_bit(input);
        input |= (replacement << 0);
    }

    template<class T>
    void lodge::lsb<T>::set_bit(T &input, T replacement, unsigned long n) {
        input &= ~(1UL << n);
        input |= replacement << n;
    }

    template<class T>
    T lodge::lsb<T>::read_lsb(T &input) {
        return (input >> 0) & 1UL;
    }

    template<class T>
    void lodge::lsb<T>::write_lsb_array(T *input, T *replacement) {
        size_t rep_size = (sizeof(replacement) / sizeof(*replacement));
        for (size_t i = 0; i < rep_size; ++i) {
            lodge::lsb<T>::write_lsb(input[i], replacement[i]);
        }
    }

    template<class T>
    std::vector<T> lodge::lsb<T>::read_lsb_array(T *input) {
        size_t s = sizeof(input);
        vector<T> ret;

        for (size_t i = 0; i < s; ++i) {
            T a = lodge::lsb<T>::read_lsb(input[i]);
            ret.push_back(a);
        }

        return ret;
    }

}

#endif //LODGE_ENCODER_H
