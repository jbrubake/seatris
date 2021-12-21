Summary: NCurses based tetris clone.
Name: seatris
Version: 0.0.14
Release: 1
Copyright: GPL
Group: Games
Source: seatris-0.0.14.tar.gz
BuildRoot: /tmp/seatris-build
Packager: Jonathan McDowell <noodles@earth.li>
Vendor: Project Purple

%description
Seatris is a basic ncurses based tetris clone. It features a high score
file, colour and configurable keys.

%changelog
* Fri Nov 26 1999 Jonathan McDowell <noodles@earth.li>
- Changed to reflect new name of seatris.
* Fri Sep 24 1999 Jonathan McDowell <noodles@earth.li>
- Changed tetris.score to be a config file to prevent overwritting when
  package is upgraded.
* Sat Jul 3 1999 Jonathan McDowell <noodles@earth.li>
- Packaged as an rpm.

%prep
%setup

%build
./configure --prefix=/usr
make

%install
mkdir -p $RPM_BUILD_ROOT/usr/games
mkdir -p $RPM_BUILD_ROOT/usr/man/man6
mkdir -p $RPM_BUILD_ROOT/var/lib/games
make install-rpm

%clean
rm -rf $RPM_BUILD_ROOT

%files
%attr(2711,root,games) /usr/games/seatris
%attr(664,root,games) %config /var/lib/games/seatris.score
%attr(644,root,root) /usr/man/man6/seatris.6
%doc README TODO LICENSE HISTORY ACKNOWLEDGEMENTS example.seatrisrc
