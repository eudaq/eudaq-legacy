#ifndef EUDAQ_INCLUDED_Logger
#define EUDAQ_INCLUDED_Logger

#include "eudaq/LogSender.hh"
#include "eudaq/LogMessage.hh"
#include "eudaq/EnvSender.hh"
#include "eudaq/EnvMessage.hh"

namespace eudaq {
  LogSender & GetLogger();
  EnvSender & GetEnvLogger();
}

#define EUDAQ_LOG_LEVEL(level) ::eudaq::GetLogger().SetLevel(level)
#define EUDAQ_ERR_LEVEL(level) ::eudaq::GetLogger().SetErrLevel(level)
#define EUDAQ_IS_LOGGED(level) ::eudaq::GetLogger().IsLogged(level)
#define EUDAQ_LOG_CONNECT(type, name, server) ::eudaq::GetLogger().Connect(type, name, server)

#define EUDAQ_LOG(level, msg) ::eudaq::GetLogger().SendLogMessage(::eudaq::LogMessage(msg, ::eudaq::LogMessage::LVL_##level) \
                                                                .SetLocation(__FILE__, __LINE__, EUDAQ_FUNC))
#define EUDAQ_DEBUG(msg) EUDAQ_LOG(DEBUG, msg)
#define EUDAQ_EXTRA(msg) EUDAQ_LOG(EXTRA, msg)
#define EUDAQ_INFO(msg)  EUDAQ_LOG(INFO, msg)
#define EUDAQ_WARN(msg)  EUDAQ_LOG(WARN, msg)
#define EUDAQ_ERROR(msg) EUDAQ_LOG(ERROR, msg)
#define EUDAQ_USER(msg)  EUDAQ_LOG(USER, msg)

#define EUDAQ_ENV(msg) ::eudaq::GetEnvLogger().SendEnvMessage(::eudaq::EnvMessage(msg, ::eudaq::EnvMessage::LVL_ENV))
#define EUDAQ_ENV_CONNECT(type, name, server) ::eudaq::GetEnvLogger().Connect(type, name, server)
//#define EUDAQ_ENV(type, dtype, name, val) ::eudaq::GetEnvLogger().SendEnv(type, dtype, name, val)

#endif // EUDAQ_INCLUDED_Logger
