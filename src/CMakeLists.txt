set(kwebcam_SRCS
   kwebcam.cpp
   main.cpp
   kwebcamview.cpp
 )

find_library(SOLID_LIBRARY solid)
if(NOT SOLID_LIBRARY)
   message(ERROR "please specify the library for SOLID")
endif(NOT SOLID_LIBRARY)

kde4_add_ui_files(kwebcam_SRCS kwebcamview_base.ui prefs_base.ui)

kde4_add_kcfg_files(kwebcam_SRCS settings.kcfgc )

kde4_add_executable(kwebcam ${kwebcam_SRCS})

include(${QT_USE_FILE})

target_link_libraries(kwebcam ${KDE4_KDEUI_LIBS} ${SOLID_LIBRARY} ${PHONON_LIBRARY})

install(TARGETS kwebcam ${INSTALL_TARGETS_DEFAULT_ARGS} )


########### install files ###############

install( PROGRAMS kwebcam.desktop  DESTINATION ${XDG_APPS_INSTALL_DIR} )
install( FILES kwebcam.kcfg  DESTINATION  ${KCFG_INSTALL_DIR} )
install( FILES kwebcamui.rc  DESTINATION  ${DATA_INSTALL_DIR}/kwebcam )

