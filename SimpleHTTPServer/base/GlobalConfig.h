//
// Created by lengyu on 2019/3/21.
//

#ifndef SIMPLEHTTPSERVER_CONFIG_H
#define SIMPLEHTTPSERVER_CONFIG_H

#include <LogStream.h>

namespace SimpleServer {
    namespace Config {
        void parseConfig();

        namespace detail {
            class __Config {
            };

            class CacheConfig : public __Config {
            private:
                bool status;
                size_t cacheSize;
                size_t cacheNodeSize;
                size_t expired;
            public:
                friend SimpleServer::detail::LogStream &
                operator<<(SimpleServer::detail::LogStream &out, CacheConfig &cacheConfig);

                friend void SimpleServer::Config::parseConfig();

                bool Status() { return this->status; }

                size_t CacheSize() { return this->cacheSize; }

                size_t CacheNodeSize() { return this->cacheNodeSize; }

                size_t Expired() { return this->expired; }
            };

            class LogConfig : public __Config {
            private:
                bool status;
                //SimpleServer::Logger::LOG_LEVEL level;
                size_t loggerBufferSize;
                size_t fullVectorSize;
                size_t emptyVectorSize;
                std::string logFilePath;
            public:
                friend SimpleServer::detail::LogStream &
                operator<<(SimpleServer::detail::LogStream &out, LogConfig &logConfig);

                friend void SimpleServer::Config::parseConfig();

                bool Status() { return this->status; };

//                Logger::LOG_LEVEL Level(){return this->level;};
                size_t LoggerBufferSize() { return this->loggerBufferSize; };

                size_t FullVectorSize() { return this->fullVectorSize; }

                size_t EmptyVectorSize() { return this->emptyVectorSize; }

                std::string &LogFilePath() { return this->logFilePath; }
            };

            class ServerConfig : public __Config {
            private:
                std::string domain;
                size_t listenPort;
                std::string root;
                std::string index;
            public:
                friend SimpleServer::detail::LogStream &
                operator<<(SimpleServer::detail::LogStream &out, ServerConfig &serverConfig);

                friend void SimpleServer::Config::parseConfig();

                std::string &Domain() { return this->domain; }

                size_t ListenPort() { return this->listenPort; }

                std::string &Root() { return this->root; }

                std::string &Index() { return this->index; }
            };
            class OtherConfig: public __Config{
            private:
                size_t workerThread;
            public:
                friend void SimpleServer::Config::parseConfig();
                const size_t getWorkerThread(){
                    return this->workerThread;
                }
            };
            class __GlobalConfig {
            public:
                friend SimpleServer::detail::LogStream &
                operator<<(SimpleServer::detail::LogStream &out, __GlobalConfig &globalConfig);

                friend void SimpleServer::Config::parseConfig();

                CacheConfig &Cache() { return this->__cache; }

                LogConfig &Log() { return this->__log; }

                std::vector<ServerConfig> &Server() { return this->__server; }
                OtherConfig &Other(){ return this->__other;}
            private:
                CacheConfig __cache;
                LogConfig __log;
                std::vector<ServerConfig> __server;
                OtherConfig __other;
            };
        }
    }
    extern Config::detail::__GlobalConfig GlobalConfig;
}
#endif //SIMPLEHTTPSERVER_CONFIG_H
