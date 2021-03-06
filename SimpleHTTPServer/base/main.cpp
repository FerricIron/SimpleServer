//
// Created by lengyu on 2019/3/2.
//
#include <Cache.h>
#include <csignal>
#include "nethelp.h"
#include "Logger.h"
#include "Logging.h"
#include "ListenServer.h"
#include "CurrentThread.h"
#include "WorkerThread.h"
#include "syscall.h"
#include "GlobalConfig.h"

namespace SimpleServer {

    void *WorkerFunction(void *args) {
        __threadName = __ThreadNameStorage[0];
        __threadStringSize = strlen(__threadName);
        __FormatString();
        LOG_INFO << "Start Worker Thread";
        auto workerThread = static_cast<WorkerThread *>(args);
        workerThread->start();
        LOG_INFO << "End Worker Thread";
        return nullptr;
    }

    void *LoggerFunction(void *args) {
        __threadName = __ThreadNameStorage[2];
        __threadStringSize = strlen(__threadName);
        __FormatString();
        LOG_INFO << "Start Log Thread";
        Logger::LoggerThread thread;
        thread.Start();
        return nullptr;
    }

    int main() {
        signal(SIGPIPE,SIG_IGN);
        LOG_INFO << "Start Server";
#ifdef __DEBUG__
        LOG_INFO << "Debug";
        signal(SIGSEGV,Logger::signal_handler);
#endif
        EventLoop<HTTPTask> loop;
        const int MAX_WORKER_SIZE=GlobalConfig.Other().getWorkerThread()<=1?1:GlobalConfig.Other().getWorkerThread();
        LOG_INFO<<" worker thread number:"<<MAX_WORKER_SIZE;
        Mutex __lock;
        ConditionLock conditionLock(__lock);
        ListenServer server(256, &conditionLock, &loop);
        pthread_t tid;
        pthread_create(&tid, nullptr, LoggerFunction, nullptr);
        for (size_t i = 0; i < MAX_WORKER_SIZE; ++i) {
            WorkerThread *work = new WorkerThread(i,&conditionLock,&loop);
            pthread_create(&tid, nullptr, WorkerFunction, (void *) (work));
        }
        server.StartListening();
        return 0;
    }
}

int main() {
    SimpleServer::Config::parseConfig();
    return SimpleServer::main();
}
