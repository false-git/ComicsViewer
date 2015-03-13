# Comics Viewer
Comics Viewer by Qt

## Requirements
* Qt 5.4.0
* quazip 0.7.1 (http://quazip.sourceforge.net/)

## How to build
### build quazip
1. get quazip-0.7.1.tar.gz from http://sourceforge.net/projects/quazip/
2. extract quazip source
<pre>
% tar xzf quazip-0.7.1.tar.gz
</pre>
3. patch quazip-0.7.1/quazip/quazip.pro
<pre>
% cd quazip-0.7.1
% wget https://gist.githubusercontent.com/false-git/b83f4ab8c3fc6759d684/raw/58855b77d34b5b53ca93dbb6cf55c264730238fa/quazip.patch
% patch -p0 < quazip.patch
</pre>
4. build and install quazip
<pre>
% qmake PREFIX=$HOME/qt/quazip
% make
% make install
</pre>

### build ComicsViewer
<pre>
% cd ComicsViewer
% qmake QUAZIP_DIR=$HOME/qt/quazip
% make
</pre>

