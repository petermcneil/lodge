#ifndef LODGE_LEASTSIGBIT_H
#define LODGE_LEASTSIGBIT_H
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <libavutil/avutil.h>
};

#include <string>
#include <stdlib.h>

using namespace std;
namespace lodge {

    class Encoder {

    private:
        Encoder() = default;
    };


    template<class T>
    class lsb : public Encoder {

    private:
        lsb() = default;

    public:
        static void flatten_bit(T &input);

        static void write_lsb(T &input, T replacement);

        static T read_lsb(T &input);

        static void encode_data(T *rgb[], T *replacement[]);

        static int insert_into_frame(AVFrame *frame, string s);

        static int extract_from_frame(AVFrame *frame);
    };

    template<class T>
    int lodge::lsb<T>::insert_into_frame(AVFrame *frame, string s) {
        return 0;
    }

    template<class T>
    int lodge::lsb<T>::extract_from_frame(AVFrame *frame) {
        return 0;
    }

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
    T lodge::lsb<T>::read_lsb(T &input) {
        return (input >> 0) & 1UL;
    }

}


#endif //LODGE_LEASTSIGBIT_H
