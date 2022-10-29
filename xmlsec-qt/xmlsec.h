#ifndef  QXMLSEC_H
# define QXMLSEC_H

# include "xmlsec-qt_global.h"
# include <QObject>
# include <QDomDocument>

class XMLSECQT_EXPORT QXmlSec : public QObject
{
public:
  explicit QXmlSec(QObject* parent = nullptr);
  ~QXmlSec();
};

#endif
