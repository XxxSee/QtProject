#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <algorithm>
#include <atomic>
#include <deque>
#include <vector>
#include <chrono>
#include <memory>
#include <QDebug>

using namespace std;

class ThreadPool
{
public:
    using Fun = void ();
    static ThreadPool *ins();
    ThreadPool():m_dThreadNum(0),m_bRun(false){}
    ~ThreadPool() {
        stop();
    }
    bool isRunning() {
        return m_bRun.load();
    }
    void start(int num = 15) {
        m_dThreadNum = num;
        m_bRun.store(true);
        for (int i{0}; i<m_dThreadNum; ++i) {
            m_vThreads.emplace_back(new thread(bind(&ThreadPool::onWork,this,i)));
            m_vFunComplete.emplace_back(0);
        }
    }
    void addTask(function<Fun> f) {
        unique_lock<mutex> lock(m_mtx);
        m_dTasks.push_back(f);
        lock.unlock();
        m_cv.notify_all();
    }
    void wait() {
        while (m_bRun.load() && !m_dTasks.empty()) {
            this_thread::sleep_for(chrono::milliseconds(100));
        }
        //任务清空了，但是正在执行的任务函数没有执行结束
        for (;;) {
            int flag = 0;
            for (int i{0}; i<m_vFunComplete.size() && m_bRun.load(); i++) {
                flag |= m_vFunComplete[i];
            }
            if (0 == flag) {
                break;
            }
            this_thread::sleep_for(chrono::milliseconds(100));
        }

        for (int i{0}; i<m_vFunComplete.size(); i++) {
            m_vFunComplete[i] = 0;
        }
    }
    void stop() {
        m_bRun.store(false);
        m_cv.notify_all();
        for (int i{0}; i<m_vThreads.size(); i++) {
            m_vThreads[i]->join();
        }
        m_dTasks.clear();
        m_vThreads.clear();
        m_vFunComplete.clear();
        m_dThreadNum = 0;
    }
private:
    //线程函数
    void onWork(int index) {
        while (m_bRun.load()) {
            unique_lock<mutex> lock(m_mtx);
            while (m_bRun.load() && m_dTasks.empty()) {
                m_cv.wait(lock);
            }
            if (!m_bRun.load())
                break;
            m_vFunComplete[index] = 1;
            function<Fun> f = m_dTasks.front();
            m_dTasks.pop_front();
            lock.unlock();
            m_cv.notify_all();
            f();
            m_vFunComplete[index] = 0;
        }
    }
private:
    int m_dThreadNum;
    atomic_bool m_bRun;
    deque<function<Fun>> m_dTasks;
    vector<unique_ptr<thread>> m_vThreads;
    vector<int> m_vFunComplete;
    mutex m_mtx;
    condition_variable m_cv;
    static unique_ptr<ThreadPool> m_ins;
};

#endif // THREADPOOL_H
