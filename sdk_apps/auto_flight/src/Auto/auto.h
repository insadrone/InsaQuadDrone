#ifndef AUTO_H
#define AUTO_H

#include <VP_Api/vp_api_thread_helper.h>

typedef struct f_data_t {
  //angle au nord
  float psi_current;
  //niveau de batterie
  int bat_level_current;;
  //control state
  int ctrl_state_current;
  //nb de tag detectes
  int tag_detected;
  // hauteur
  float alt;
  //tableau des tags
  uint32_t const *tag_tab;

} fdata;

PROTO_THREAD_ROUTINE(auto_control, data);

extern int auto_ready;
extern FILE *redir_sortie;

float get_alt();

inline C_RESULT auto_navdata_client_init( void* data );
inline C_RESULT auto_navdata_client_process( const navdata_unpacked_t* const navdata );
inline C_RESULT auto_navdata_client_release( void );


#endif // MOVE_H
