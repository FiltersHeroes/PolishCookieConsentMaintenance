# PolishCookieConsentUpdater
Aktualizator do rozszerzenia Polska Ciasteczkowa Zgoda dla Chromium (do Windowsa)

## Budowanie

Aby skompilować ten program, musisz mieć:

- biblioteki Qt 5, które prekompilowane możesz pobrać z [qt.io/download](https://www.qt.io/download) lub samodzielnie skompilować wg instrukcji dostępnych na [wiki.qt.io/Building_Qt_5_from_Git](https://wiki.qt.io/Building_Qt_5_from_Git)

- bibliotekę OpenSSL, którą prekompilowaną możesz pobrać ze źródeł wymienionych na [wiki.openssl.org/index.php/Binaries](https://wiki.openssl.org/index.php/Binaries), a w przypadku posiadania Windowsa polecamy tą ze strony [indy.fulgan.com/SSL/](https://indy.fulgan.com/SSL/). Jednakże możesz również sam ją skompilować wg. instrukcji z [wiki.openssl.org/index.php/Compilation_and_Installation](https://wiki.openssl.org/index.php/Compilation_and_Installation)

- bibliotekę QuaZIP, którą możesz skompilować wg. instrukcji z [stachenov.github.io/quazip/](https://stachenov.github.io/quazip/), a jeśli masz Windowsa, możesz skorzystać z instrukcji dostępnych na [www.antonioborondo.com/zipping-and-unzipping-files-with-qt/](https://www.antonioborondo.com/zipping-and-unzipping-files-with-qt/).

Jak już wszystko uzyskasz, otwórz plik [PolishCookieConsentUpdater.pro](https://raw.githubusercontent.com/PolishFiltersTeam/PolishCookieConsentUpdater/master/PolishCookieConsentUpdater.pro) w programie Qt Creator i kliknij na ikonę trójkąta w celu kompilacji i uruchomienia.

## **Podziękowania**

Ten program korzysta z otwartoźródłowego kodu następujących projektów (wg. kolejności alfabetycznej):

- [QuaZIP](https://github.com/stachenov/quazip), który jest na licencji [LGPL](/other_licenses/QuaZIP_LICENSE.txt)
- [Qt 5](https://github.com/qt/qt5) na licencji [GPL](/other_licenses/Qt_LICENSE.txt)
- [OpenSSL](https://github.com/openssl/openssl), który jest na [podwójnej licencji](/other_licenses/OpenSSL_LICENSE.txt)

