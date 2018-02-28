#ifndef RINGBUFFER_H
#define RINGBUFFER_H
// http://stackoverflow.com/questions/9743605/thread-safe-implementation-of-circular-buffer

#ifndef Q_MOC_RUN
#include <boost/thread/condition.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <boost/circular_buffer.hpp>
#endif
template <typename T>
class RingBuffer : private boost::noncopyable
{
public:
    typedef boost::mutex::scoped_lock lock;
    RingBuffer() {}
    RingBuffer(int n) {cb.set_capacity(n);}

    void send (T imdata)
    {
        lock lk(monitor);
        if (cb.size() > 0)
            cb.pop_back();
        cb.push_back(imdata);
        buffer_not_empty.notify_one();
    }

    template<typename Duration>
    T receive(Duration timeout, bool *bok) {//T receive() {
        lock lk(monitor);
        /*while (cb.empty())
            buffer_not_empty.wait(lk); // buffer_not_empty.timed_wait(lk,timeout);
            */
        while (cb.empty())
        {
            if (false == buffer_not_empty.timed_wait(lk,boost::posix_time::milliseconds(timeout)))
            {
                *bok = false;
                T imdata;
                return imdata;
            }
        }
        *bok = true;
        T imdata = cb.front();
        cb.pop_front();
        return imdata;
    }
    void clear() {
        lock lk(monitor);
        cb.clear();
    }
    int size() {
        lock lk(monitor);
        return cb.size();
    }
    void set_capacity(int capacity) {
        lock lk(monitor);
        cb.set_capacity(capacity);
    }
private:
    boost::condition buffer_not_empty;
    boost::mutex monitor;
    boost::circular_buffer<T> cb;
};
#endif // RINGBUFFER_H
