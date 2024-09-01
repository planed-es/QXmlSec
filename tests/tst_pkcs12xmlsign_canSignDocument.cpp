#include "tst_pkcs12xmlsign.h"

void Pkcs12XmlSignTest::canSignDocument()
{
  QXmlSec xmlSec; // required to initialize and finalize libxmlsec1
  QXmlSecCertificate certificate;
  QXmlSign signer;
  QString keyInfoId, signatureSignInfoRefId;
  QSslKey sslKey;
  QSslCertificate noop;

  QVERIFY(prepareCertificateFile());
  QVERIFY(prepareXmlDocument());
  QVERIFY(QSslCertificate::importPkcs12(&certificateFile, &sslKey, &noop, nullptr, password.toUtf8()));

  certificate.setFormat(QXmlSecCertificate::Pkcs12);
  certificate.setFilepath(certificateFile.fileName());
  certificate.setPassword(password);

  signer.withSignatureId("Signature")
    .useNamespace("testSignature")
    .useDocument(xmlDocument)
    .useSslKey(sslKey, password.toUtf8())
    .useCertificate(certificate);

  keyInfoId = signer.signatureContext().tagId("KeyInfo");
  signatureSignInfoRefId = signer.signatureContext().tagId("Reference");

  signer.useSignInfo(QXmlSignInfo()
    .addReference(QXmlSignReference()
      .withType(QUrl("http://www.w3.org/2000/09/xmldsig#Object"))
      .useAlgorithm(QUrl("http://www.w3.org/2001/04/xmlenc#sha512"))
    //.useAlgorithm(QUrl("http://www.w3.org/2000/09/xmldsig#Object")) // that algorithm doesn't work, it might be good to find out why
      .addTransform(QUrl("http://www.w3.org/2000/09/xmldsig#enveloped-signature"))
      .addTransform(QUrl("http://www.w3.org/TR/1999/REC-xpath-19991116"))
    )
    .addReference(QXmlSignReference()
      .withUri('#' + keyInfoId)
      .useAlgorithm(QUrl("http://www.w3.org/2001/04/xmlenc#sha512"))
    )
  );

  signer.useKeyInfo(QXmlSignKeyInfo()
    .withId(keyInfoId)
  );


  QVERIFY(signer.sign());
}
