#ifndef AUTO_H
#define AUTO_H

#include <VP_Api/vp_api_thread_helper.h>

PROTO_THREAD_ROUTINE(auto_control, data);

inline C_RESULT auto_navdata_client_init( void* data );
inline C_RESULT auto_navdata_client_process( const navdata_unpacked_t* const navdata );
inline C_RESULT auto_navdata_client_release( void );


#endif // MOVE_H
