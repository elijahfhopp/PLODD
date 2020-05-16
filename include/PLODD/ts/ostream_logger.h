////////////////////////////////////////////////////////////////////////////////
/// @file ostream_logger.h
/// @author Elijah Hopp (github.mrzeusthecoder@gmail.com)
/// @brief 
/// @version 0.0.0
/// @date 2020-02-25
/// 
/// @copyright Copyright (c) 2020 Elijah Hopp, No Rights Reserved
/// 
////////////////////////////////////////////////////////////////////////////////
//--------------------------------DEFINE_GUARD--------------------------------//
#ifndef PLODD_TS_OSTREAM_LOGGER_H_
#define PLODD_TS_OSTREAM_LOGGER_H_
//----------------------------------INCLUDES----------------------------------//
#include "../basic/base.h"

#include <iostream>
#include <string>
#include <mutex>
#include <list>
//-------------------------THREAD_SAFE_OSTREAM_LOGGER-------------------------//
namespace pld {
namespace ts {

////////////////////////////////////////////////////////////////////////////////
/// @brief An internal use type for locking ostream objects in the ts::ostream_logger.
////////////////////////////////////////////////////////////////////////////////
struct _shared_stream_mutex {
    std::ostream * stream_ptr;
    //The mutex pointed to is actually heaped by the ostream_logger ctor.
    std::mutex * mutex;
    //Technically only allows for 256 loggers. Who would do something like that... Hey, look a new issue... oh..
    uint8_t dependents;
    _shared_stream_mutex() = delete;
    _shared_stream_mutex(std::ostream * stream_ptr, std::mutex * mutex_ptr, uint8_t dependents) : stream_ptr(stream_ptr), mutex(mutex_ptr), dependents(dependents){}
};

////////////////////////////////////////////////////////////////////////////////
/// @brief A thread safe ostream logger.
/// @details This logger logs to an ostream object and is thread 
/// safe across multipule loggers. That means four loggers spread 
/// over 16 threads all trying to access the same object will be 
/// safely locked. That is an extreme case, but it can handle it 
/// nonetheless.
/// @see pld::ostream_logger
////////////////////////////////////////////////////////////////////////////////
class PLODD_API ostream_logger : public base_logger {
    protected:
        //Static:
        static std::list<_shared_stream_mutex> shared_mutex_list;
        static std::mutex vector_locking_mutex;
        //Per-object:
        std::ostream * stream_ptr;
        _shared_stream_mutex * shared_ostream_mutex;
    public:
        ostream_logger(std::ostream * stream_pointer);
        ~ostream_logger();
        void set_stream();
        void debug(std::string msg);
        void info(std::string msg);
        void warn(std::string msg);
        void error(std::string msg);
};

} //ts
} //pld
//-----------------------------------END_IF-----------------------------------//
#endif //PLODD_TS_OSTREAM_LOGGER_H_