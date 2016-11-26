#!/bin/sh

function _die() {
	cat $1 >&3
	exit 1
}

function check_compile1() {
	test -d bin/$1 && rm -rf bin/$1
	mkdir bin/$1
	pushd bin/$1 >/dev/null 2>&1
	shift
	exec 3>&1
	exec >cibuild.log
	../../configure "$@" 2>&1 || _die cibuild.log
	make check 2>&1 || _die cibuild.log
	popd >/dev/null 2>&1
	exec >&3
}

function run_job() {
	if [ "$no_parallel" = 0 ]; then
		echo misc/cibuild.sh -c "$@" >&9
	else
		echo "$@..."
		check_compile1 "$@"
	fi
}

function check_compile() {
	local name
	name="$1"
	shift
	run_job "${name}-debug" "$@" --enable-debug
	run_job "${name}-release" "$@"
}

function has_cmd() {
	which $1 >/dev/null 2>&1
}

if test "$1" = "-c"; then
	shift
	check_compile1 "$@"
	exit $?
fi

cd "$(dirname "$0")/.."

if [ ${no_parallel:-0} = 0 ]; then
	if which parallel >/dev/null 2>&1; then
		echo 'using parallel'
		test -e .cibuild.pipe && rm .cibuild.pipe
		mkfifo .cibuild.pipe
		parallel --halt=1 --progress -v <.cibuild.pipe &
		parallel_pid=$!
		exec 9>.cibuild.pipe
		rm .cibuild.pipe
		no_parallel=0
	else
		no_parallel=1
	fi
fi

#./autogen.sh -v
test -d bin || mkdir bin

has_cmd x86_64-w64-mingw32-gcc && check_compile cibuild-mingw-x86_64 --host=x86_64-w64-mingw32
has_cmd i686-w64-mingw32-gcc && check_compile cibuild-mingw-i686 --host=i686-w64-mingw32
check_compile cibuild-native+psg
check_compile cibuild-native+sg --without-libpng
check_compile cibuild-native+pg --without-libsixel
check_compile cibuild-native+g  --without-libsixel --without-libpng
check_compile cibuild-native+ps --without-gtk3
check_compile cibuild-native+s  --without-gtk3 --without-libpng
check_compile cibuild-native+p  --without-gtk3 --without-libsixel
check_compile cibuild-native    --without-gtk3 --without-libsixel --without-libpng
check_compile cibuild-native+cpsg CC=clang
check_compile cibuild-native+csg  CC=clang --without-libpng
check_compile cibuild-native+cpg  CC=clang --without-libsixel
check_compile cibuild-native+cg   CC=clang --without-libsixel --without-libpng
check_compile cibuild-native+cps  CC=clang --without-gtk3
check_compile cibuild-native+cs   CC=clang --without-gtk3 --without-libpng
check_compile cibuild-native+cp   CC=clang --without-gtk3 --without-libsixel
check_compile cibuild-native+c    CC=clang --without-gtk3 --without-libsixel --without-libpng
check_compile cibuild-native+xx  CC=g++ CFLAGS='-std=c++11' --without-gtk3 --without-libsixel --without-libpng

if ${parallel_pid+:} false; then
	exec 9>&-
	wait ${parallel_pid}
	exit $?
fi
