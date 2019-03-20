#!/bin/bash

# Create symlinks
if [[ $LANG == *"pl"* ]];
then
echo "Tworzenie symlinków do ikon (wymagane są prawa roota)..."
else
echo "Creating symlinks to icons (root priveleges are required)..."
fi
sudo ln -sf $USER_PWD/PolishCookieConsentMaintenance/icons/icon16.png /usr/share/icons/hicolor/16x16/apps/PolishCookieConsentMaintenance.png
sudo ln -sf $USER_PWD/PolishCookieConsentMaintenance/icons/icon24.png /usr/share/icons/hicolor/24x24/apps/PolishCookieConsentMaintenance.png
sudo ln -sf $USER_PWD/PolishCookieConsentMaintenance/icons/icon32.png /usr/share/icons/hicolor/32x32/apps/PolishCookieConsentMaintenance.png
sudo ln -sf $USER_PWD/PolishCookieConsentMaintenance/icons/icon48.png /usr/share/icons/hicolor/48x48/apps/PolishCookieConsentMaintenance.png
sudo ln -sf $USER_PWD/PolishCookieConsentMaintenance/icons/icon128.png /usr/share/icons/hicolor/128x128/apps/PolishCookieConsentMaintenance.png
sudo ln -sf $USER_PWD/PolishCookieConsentMaintenance/icons/icon128.png /usr/share/pixmaps/PolishCookieConsentMaintenance.png

if [[ $LANG == *"pl"* ]];
then
echo "Tworzenie skrótów (wymagane są prawa roota)..."
else
echo "Creating shortcuts (root priveleges are required)..."
fi

# Create symlink to binary
sudo install -Dm755 /dev/stdin "/usr/bin/PolishCookieConsentMaintenance" <<END
#!/bin/sh
exec $USER_PWD/PolishCookieConsentMaintenance/PolishCookieConsentMaintenance.sh "\$@"
END

# Create start menu shortcut
sudo install -Dm644 /dev/stdin "/usr/share/applications/PolishCookieConsentMaintenance.desktop" <<EOF
[Desktop Entry]
Version=1.0
Name=Polish Cookie Consent Maintenance
Name[pl]=Konserwator Polskiej Ciasteczkowej Zgody
Comment=Install or update Polish Cookie Consent extension for Chromium
Comment[pl]=Zainstaluj lub zaktualizuj rozszerzenie Polska Ciasteczkowa Zgoda dla Chromium
GenericName=Maintenance tool
GenericName[pl]=Narzędzie konserwacyjne
Keywords=Internet;WWW;Web;
Keywords[pl]=Internet;WWW;Sieć;
Exec=PolishCookieConsentMaintenance %u
Terminal=false
X-MuiltpleArgs=false
Type=Application
Icon=PolishCookieConsentMaintenance
Categories=Network;Settings;
StartupNotify=true
EOF
         
if [[ $LANG == *"pl"* ]];
then
Yes="Tak"
No="Nie"
echo "Czy chcesz ustawić automatyczne cotygodniowe sprawdzanie i pobieranie aktualizacji (wymagane są prawa roota)?"
else
Yes="Yes"
No="No"
echo "Do you wish to set automatic weekly checking and downloading of updates (root priveleges are required)?"
fi

select yn in $Yes $No; do
case $yn in
$Yes ) 
cat > /tmp/update_PolishCookieConsent_chromium << EOF
#!/bin/sh
export DISPLAY=:0.0
$USER_PWD/PolishCookieConsentMaintenance/PolishCookieConsentMaintenance.sh /u
EOF
sudo mv /tmp/update_PolishCookieConsent_chromium /etc/cron.weekly/
sudo chmod +x /etc/cron.weekly/update_PolishCookieConsent_chromium
break;;
$No ) exit 0; break;;
esac
done

rm -r $USER_PWD/PolishCookieConsentMaintenance/postInstall.sh

