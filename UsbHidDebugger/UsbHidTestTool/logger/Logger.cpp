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
**  \file Logger.cpp
**  \author Ln
**
********************************************************************************
**
**  \brief This file handle log
**
********************************************************************************
**
*/
#include "pch.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <map>
#include <algorithm> //std::transform
#include <cctype>    // toupper
#include "Logger.hpp"
#include "./configIni/inicpp.h"

namespace
{
    using namespace easylog;

    const std::string g_configFile{ "easylog.config" };

    std::once_flag singletonFlag;

    const std::string g_logFile{ "easylog.log" };

    LOG_LEVEL StringToLevel(const std::string& s)
    {
        static const std::map<std::string, LOG_LEVEL> Str2Level {
            {"DEBUG",     LOG_LEVEL::DBG},
            {"DEFAULT",   LOG_LEVEL::DEFAULT},
            {"INFO",      LOG_LEVEL::INFO},
            {"WARN",      LOG_LEVEL::WARN},
            {"ERROR",     LOG_LEVEL::ERR},
            {"CRITICAL",  LOG_LEVEL::CRITICAL}
        };

        std::string str{ s };
        std::transform(str.begin(), str.end(), str.begin(),
            [](unsigned char c) { return std::toupper(c); });

        auto iter = Str2Level.find(str);
        if (iter == Str2Level.end())
        {
            return LOG_LEVEL::DEFAULT;
        }

        return iter->second;
    }

    std::string getLocalTime()
    {
        const std::string TIME_FORMAT{ "%Y-%m-%d %H:%M:%S" };
        std::stringstream ss;
        auto now = std::chrono::system_clock::now();
        uint64_t ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count()
            - std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count() * 1000;

        time_t t = std::chrono::system_clock::to_time_t(now);
        struct tm st;
        localtime_s(&st, &t);
        ss << std::put_time(&st, TIME_FORMAT.c_str());
        ss << "." << ms;
        std::string tmStr = ss.str();
        return tmStr;
    }

    std::string getFilename(const std::string& path)
    {
        size_t pos = path.find_last_of("\\/");
        if (pos == std::string::npos)
        {
            return path;
        }

        std::string filename = path.substr(pos + 1);
        return filename;
    }

}

namespace easylog
{
    // class Logger
    std::shared_ptr<Logger> Logger::s_instance = nullptr;

    Logger::~Logger()
    {
        if (m_logStream.is_open())
        {
            m_logStream.close();
        }
    }

    void Logger::ParseConfig()
    {
    #ifdef EASYLOG_CONFIG_FILE
        m_logConfFile = EASYLOG_CONFIG_FILE;
    #else
        m_logConfFile = g_configFile;
    #endif
        LOG_DBG("ParseConfig: config file path: " << m_logConfFile);

        ini::IniFile logConf;
        try
        {
            logConf.load(m_logConfFile);
        }
        catch (const std::exception&)
        {
            LOG_DBG("ParseConfig: load ini fail, inifile = " << m_logConfFile);
            m_logConfs.logLevel = LOG_LEVEL::DEFAULT;
            m_logConfs.srcFileLine = false;
            m_logConfs.logFile = g_logFile;
            return;
        }

        try
        {
            std::string lvl = logConf["Log_Config"]["LogLevel"].as<std::string>();
            m_logConfs.logLevel = StringToLevel(lvl);
        }
        catch (const std::exception&)
        {
            LOG_DBG("ParseConfig: get log level fail!");
            m_logConfs.logLevel = LOG_LEVEL::DEFAULT;
        }

        try
        {
            m_logConfs.srcFileLine = logConf["Log_Config"]["LogSrcFileLine"].as<bool>();
        }
        catch (const std::exception&)
        {
            LOG_DBG("ParseConfig: get Log SrcFileLine fail!");
            m_logConfs.srcFileLine = false;
        }

        try
        {
            m_logConfs.logFile = logConf["Log_Config"]["TargetLogFile"].as<std::string>();
        }
        catch (const std::exception&)
        {
            LOG_DBG("ParseConfig: get Log file conf fail!");
            m_logConfs.logFile = g_logFile;
        }

        LOG_DBG("ParseConfig: logLevel = " << static_cast<int>(m_logConfs.logLevel));
        LOG_DBG("ParseConfig: srcFileLine = " << m_logConfs.srcFileLine);
        LOG_DBG("ParseConfig: logFile = " << m_logConfs.logFile);
    }

    void Logger::LogInit()
    {
        ParseConfig();

        if (!m_logStream.is_open())
        {
            m_logStream.open(m_logConfs.logFile, std::ios::out | std::ios::app);

            if (!m_logStream.is_open())
            {
                LOG_DBG("LogInit: open log file failed!");
                m_logConfs.logFile = g_logFile;
                m_logStream.open(m_logConfs.logFile, std::ios::out | std::ios::app);
            }

            LOG_DBG("LogInit: logFile = " << m_logConfs.logFile);
        }
    }

    std::shared_ptr<Logger> Logger::getInstance()
    {
        std::call_once(singletonFlag, [&] {
            s_instance = std::shared_ptr<Logger>(new Logger());
            s_instance->LogInit();
        });

        return s_instance;
    }

    void Logger::Logging(std::string file, int line, LOG_LEVEL lvl, std::string str)
    {
        std::lock_guard<std::mutex> lck(m_mutex);

        if (!m_logStream.is_open() || !m_logStream.good())
        {
            LOG_DBG("Logging: log file not ready!");
            return;
        }

        if (lvl < m_logConfs.logLevel)
        {
            LOG_DBG("Logging: level not ok, lvl=" << static_cast<int>(lvl) << ", configed level = " << static_cast<int>(m_logConfs.logLevel));
            return;
        }

        if (m_logConfs.srcFileLine)
        {
            m_logStream << "[" << getFilename(file) << ":" << line << "]" << getLocalTime() << " " << str << std::endl;
        }
        else
        {
            m_logStream << getLocalTime() << " " << str << std::endl;
        }

        m_logStream.flush();
    }

    void Logger::Console(std::string file, int line, LOG_LEVEL lvl, std::string str)
    {
        std::lock_guard<std::mutex> lck(m_mutex);

        if (lvl < m_logConfs.logLevel)
        {
            LOG_DBG("Console: level not ok, lvl=" << static_cast<int>(lvl) << ", configed level = " << static_cast<int>(m_logConfs.logLevel));
            return;
        }

        if (m_logConfs.srcFileLine)
        {
            std::cout << "[" << getFilename(file) << ":" << line << "]" << getLocalTime() << " " << str << std::endl;
        }
        else
        {
            std::cout << getLocalTime() << " " << str << std::endl;
        }

        std::cout.flush();
    }

} // namespace

