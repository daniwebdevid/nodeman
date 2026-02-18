# Maintainer: dany saputra <danysaputra2682010@gmail.com>
pkgname=ndm
pkgver=2.6.1
pkgrel=1
pkgdesc="Native Node.js Version Manager in C"
arch=('x86_64')
url="https://github.com/daniwebdevid/nodeman"
license=('GPL')
depends=('curl' 'tar' 'xz')
makedepends=('cmake' 'make' 'gcc')
install=ndm.install

source=("${pkgname}::.")
sha256sums=('SKIP')

build() {
  cmake -B build -S "${startdir}" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=/usr
  cmake --build build
}

package() {
  install -Dm755 "build/ndm" "$pkgdir/usr/bin/ndm"

  install -dm755 "$pkgdir/opt/nodeman/config"
  
  if [ -d "${startdir}/config" ]; then
    cp -rp "${startdir}/config/"* "$pkgdir/opt/nodeman/config/"
  fi

  install -dm755 "$pkgdir/etc/profile.d"
  ln -sf "/opt/nodeman/config/profile.d.sh" "$pkgdir/etc/profile.d/nodeman.sh"

  install -dm777 "$pkgdir/var/cache/nodeman"
}