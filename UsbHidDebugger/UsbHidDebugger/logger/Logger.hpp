/**
********************************************************************************
**
**  Copyright (C) 2025 Eaton
**
**  This software is confidential and licensed under Eaton Proprietary License
**  (EPL or EULA).
**  This software is not authorized to be used, duplicated or disclosed to
**  anyone without the prior written permission of Eaton.
**  Limitations, restrictions and exclusions of the Eaton applicable standard
**  terms and conditions, such as its EPL and EULA, apply.
**
********************************************************************************
**
**  \file Logger.hpp
**  \author Ln
**
********************************************************************************
**
**  \brief This file handle log
**
**  EASYLOG_CONFIG_FILE: customize log config file path, default is ./easylog.config if the mocro not defined
**  also config file path can be get from environment variable with same name(EASYLOG_CONFIG_FILE) ----TODO
**  
**  the config file priority is: environment variable(EASYLOG_CONFIG_FILE) > macro define(EASYLOG_CONFIG_FILE) > dfault config filename(./easylog.config)
**  macro define(EASYLOG_CONFIG_FILE) can only be set before complie
**
********************************************************************************
**
*/
#pragma once

#include "pch.h"
#include <string>
#include <fstream>
#include <mutex>
#include <sstream>
#include <iostream>


// only for log debug
//#define DEBUG_MODE

#ifdef DEBUG_MODE
#define LOG_DBG(x)       do{                                           \
                                    std::cout << x << std::endl;       \
                                }while(0)
#else
#define LOG_DBG(x)
#endif


#define EASYLOG_CONFIG_FILE "easylog.config"

////////////////////////////////////////////////////////////////////////////////////////
namespace easylog
{
    enum class LOG_LEVEL
    {
        DBG = 0,
        INFO,
        DEFAULT = INFO,
        WARN,
        ERR,
        CRITICAL
    };

    struct LOG_CONFIGURATION
    {
        LOG_LEVEL logLevel{ LOG_LEVEL::DEFAULT };
        bool srcFileLine{ false };
        std::string logFile{ "easylog.log" };
    };
    using LOG_CONF = struct LOG_CONFIGURATION;


    class Logger final
    {
    public:
        virtual ~Logger();

        Logger(const Logger& other) = delete;
        Logger& operator=(const Logger& other) = delete;
        Logger(Logger&& other) = delete;
        Logger& operator=(Logger&& other) = delete;

        static std::shared_ptr<Logger> getInstance();

        std::string GetConfigPath() { return m_logConfFile; }
        LOG_LEVEL Level() { return m_logConfs.logLevel; }
        bool FileAndLine() { return m_logConfs.srcFileLine; }
        std::string GetLogPath() { return m_logConfs.logFile; }

        void Logging(std::string file, int line, LOG_LEVEL lvl, std::string str);
        void Console(std::string file, int line, LOG_LEVEL lvl, std::string str);

    private:
        Logger() = default;
        void ParseConfig();
        void LogInit();

    private:
        std::string m_logConfFile{ "" };
        std::ofstream m_logStream;
        LOG_CONF m_logConfs{};
        std::mutex m_mutex;
        static std::shared_ptr<Logger> s_instance;
    };


    // file log
    #define log_log(lvl, x)     do{                                                          \
                                     auto __log = easylog::Logger::getInstance();            \
                                     std::stringstream __ss;                                 \
                                     __ss << x;                                              \
                                     __log->Logging(__FILE__, __LINE__, lvl, __ss.str());    \
                                }while(0)


    #define log_debug(x)        log_log(easylog::LOG_LEVEL::DBG, x)
    #define log_info(x)         log_log(easylog::LOG_LEVEL::INFO, x)
    #define log_warn(x)         log_log(easylog::LOG_LEVEL::WARN, x)
    #define log_error(x)        log_log(easylog::LOG_LEVEL::ERR, x)
    #define log_critical(x)     log_log(easylog::LOG_LEVEL::CRITICAL, x)


    // Console log
    #define log_console(lvl, x)     do{                                                          \
                                         auto __log = easylog::Logger::getInstance();                     \
                                         std::stringstream __ss;                                 \
                                         __ss << x;                                              \
                                         __log->Console(__FILE__, __LINE__, lvl, __ss.str());    \
                                    }while(0)


    #define console_debug(x)      log_console(easylog::LOG_LEVEL::DBG, x)
    #define console_info(x)       log_console(easylog::LOG_LEVEL::INFO, x)
    #define console_warn(x)       log_console(easylog::LOG_LEVEL::WARN, x)
    #define console_error(x)      log_console(easylog::LOG_LEVEL::ERR, x)
    #define console_critical(x)   log_console(easylog::LOG_LEVEL::CRITICAL, x)

} // namespace



