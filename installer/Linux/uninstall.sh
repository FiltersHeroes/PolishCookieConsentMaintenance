#!/bin/bash

if [[ $LANG == *"pl"* ]];
then
Yes="Tak"
No="Nie"
echo "Czy na pewno chcesz odinstalować Konserwatora Polskiej Ciasteczkowej Zgody (wymagane są prawa roota)?"
else
Yes="Yes"
No="No"
echo "Are you sure you want to uninstall Polish Cookie Consent Maintenance (root priveleges are required)?"
fi

select yn in $Yes $No; do
case $yn in
$Yes ) 

MAIN_PATH=$(dirname "$0")

# Remove symlinks
if [ -L /usr/share/pixmaps/PolishCookieConsentMaintenance.png ]; then
	sudo rm -vrf /usr/share/pixmaps/PolishCookieConsentMaintenance.png;
fi

if [ -L /usr/share/icons/hicolor/16x16/apps/PolishCookieConsentMaintenance.png ]; then
	sudo rm -vrf /usr/share/icons/hicolor/16x16/apps/PolishCookieConsentMaintenance.png;
fi

if [ -L /usr/share/icons/hicolor/24x24/apps/PolishCookieConsentMaintenance.png ]; then
	sudo rm -vrf /usr/share/icons/hicolor/24x24/apps/PolishCookieConsentMaintenance.png;
fi

if [ -L /usr/share/icons/hicolor/32x32/apps/PolishCookieConsentMaintenance.png ]; then
	sudo rm -vrf /usr/share/icons/hicolor/32x32/apps/PolishCookieConsentMaintenance.png;
fi

if [ -L /usr/share/icons/hicolor/48x48/apps/PolishCookieConsentMaintenance.png ]; then
	sudo rm -vrf /usr/share/icons/hicolor/48x48/apps/PolishCookieConsentMaintenance.png;
fi

if [ -L /usr/share/icons/hicolor/128x128/apps/PolishCookieConsentMaintenance.png ]; then
	sudo rm -vrf /usr/share/icons/hicolor/128x128/apps/PolishCookieConsentMaintenance.png;
fi

# Remove shortcut
if [ -f /usr/share/applications/PolishCookieConsentMaintenance.desktop ]; then
	sudo rm -vrf /usr/share/applications/PolishCookieConsentMaintenance.desktop;
fi

# Remove wrapper
if [ -f /usr/bin/PolishCookieConsentMaintenance ]; then
	sudo rm -vrf /usr/bin/PolishCookieConsentMaintenance;
fi

# Remove cron job
if [ -f /etc/cron.weekly/update_PolishCookieConsent_chromium ]; then
	sudo rm -vrf /etc/cron.weekly/update_PolishCookieConsent_chromium;
fi

# Remove app
rm -rvf $MAIN_PATH;
break;;
$No ) exit 0; break;;
esac
done

