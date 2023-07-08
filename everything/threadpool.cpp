#include "threadpool.h"

unique_ptr<ThreadPool> ThreadPool::m_ins;
mutex g_tp_mtx;

ThreadPool* ThreadPool::ins()
{
    if (!m_ins.get()) {
        unique_lock<mutex> locker(g_tp_mtx);
        if (!m_ins.get()) {
            m_ins.reset(new ThreadPool());
        }
    }
    return m_ins.get();
}
