#ifndef _SOFTWARE_COMPONENT_API_H_
#define _SOFTWARE_COMPONENT_API_H_

#if defined(_WIN32)
#  if defined(EXPORTING_MYMATH)
#    define DECLSPEC __declspec(dllexport)
#  else
#    define DECLSPEC __declspec(dllimport)
#  endif
#else // non windows
#  define DECLSPEC
#endif

void DECLSPEC SoftwareComponent_API(void);

#endif
