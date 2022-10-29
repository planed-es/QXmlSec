#ifndef  XMLSECQT_GLOBAL_H
# define XMLSECQT_GLOBAL_H

# include <QtCore/qglobal.h>

# if defined(XMLSECQT_LIBRARY)
#  define XMLSECQT_EXPORT Q_DECL_EXPORT
# else
#  define XMLSECQT_EXPORT Q_DECL_IMPORT
#endif

# define XMLSEC_CRYPTO_OPENSSL
# define XMLSEC_NO_SIZE_T

#endif // TATAMI_GLOBAL_H

