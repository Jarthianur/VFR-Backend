#!/bin/bash

# setup vars
source bootstrap.sh
export VFRB_NAME=${VFRB_NAME_B:-vfrb}
export VFRB_PROP=${VFRB_PROP_B:-vfrb}
export VFRB_VERSION=$(cat version.txt)
export VFRB_COMPILER=${VFRB_COMPILER_B:-g++}
export BOOST_ROOT=${BOOST_ROOT_B:-}
export VFRB_EXEC_PATH=${VFRB_EXEC_PATH_B:-$PWD/target/}
export VFRB_PROP_PATH=${VFRB_PROP_PATH_B:-$PWD/target/}
export VFRB_TARGET=$VFRB_NAME-$VFRB_VERSION
export VFRB_ROOT=$PWD

##########################################
echo "... INSTALL VFRB ..."
echo ""

echo "... CECK VARIABLES ..."
echo ""
echo "WORKDIR = $VFRB_ROOT"
if [ -z "$VFRB_NAME_B" ]; then
    echo "VFRB_NAME not set, using default: $VFRB_NAME"
else
    echo "VFRB_NAME:" $VFRB_NAME
fi
if [ -z "$VFRB_PROP_B" ]; then
    echo "VFRB_PROP not set, using default: $VFRB_PROP"
else
    echo "VFRB_PROP: $VFRB_PROP"
fi
if [ -z "$VFRB_VERSION" ]; then
    echo "VFRB_VERSION not set"
else
    echo "VFRB_VERSION: $VFRB_VERSION"
fi
if [ -z "$VFRB_COMPILER_B" ]; then
    echo "VFRB_COMPILER not set, using default: $VFRB_COMPILER"
else
    echo "VFRB_COMPILER: $VFRB_COMPILER"
fi
if [ -z "$BOOST_ROOT_B" ]; then
    echo "BOOST_ROOT not set, assuming default"
else
    echo "BOOST_ROOT: $BOOST_ROOT"
    export BOOST_LIBS_L="-L$BOOST_ROOT/stage/lib"
    export BOOST_ROOT_I="-I$BOOST_ROOT"
fi
if [ -z "$VFRB_EXEC_PATH_B" ]; then
    echo "VFRB_EXEC_PATH not set, using default: $VFRB_EXEC_PATH"
else
    echo "VFRB_EXEC_PATH: $VFRB_EXEC_PATH"
fi
if [ -z "$VFRB_PROP_PATH_B" ]; then
    echo "VFRB_PROP_PATH not set, using default: $VFRB_PROP_PATH"
else
    echo "VFRB_PROP_PATH: $VFRB_PROP_PATH"
fi
unset VFRB_NAME_B
unset VFRB_PROP_B
unset VFRB_COMPILER_B
unset BOOST_ROOT_B
unset VFRB_EXEC_PATH_B
unset VFRB_PROP_PATH_B
echo ""
echo "... RUN MAKE ..."
echo ""
pushd $VFRB_ROOT/target/
make
error=$?
unset BOOST_LIBS_L
unset BOOST_ROOT_I
popd
if [ $error -ne 0 ]; then
    echo "ERROR: make failed $error"
    exit $error
fi
echo ""
echo "... COPY TARGETS ..."
echo ""
if [ -x "$VFRB_ROOT/target/$VFRB_TARGET" ]; then
    mv $VFRB_ROOT/target/$VFRB_TARGET $VFRB_EXEC_PATH/$VFRB_TARGET
    echo ""
    echo "$VFRB_TARGET copied to $VFRB_EXEC_PATH"
    echo ""
else
    echo ""
    echo "ERROR: $VFRB_TARGET does not exist"
    exit 1
fi
sed "s|%VERSION%|$VFRB_VERSION|" <$VFRB_ROOT/vfrb.properties >$VFRB_PROP_PATH/$VFRB_PROP.properties
echo ""
echo "$VFRB_PROP.properties copied to $VFRB_PROP_PATH"
echo ""
echo "... SETUP SERVICE ..."
echo ""
mkdir -p $VFRB_ROOT/target/service
mkdir -p $VFRB_ROOT/target/service/$VFRB_NAME.service.d
pushd $VFRB_ROOT/target/service/$VFRB_NAME.service.d
sed "s|%BOOST_LIBS_PATH%|$BOOST_ROOT/stage/lib:|" <$VFRB_ROOT/service/vfrb.service.d/vfrb.conf >$VFRB_NAME.conf
cd ..
sed -e "s|%VFRB_NAME%|$VFRB_NAME|" \
    -e "s|%VFRB_EXEC_PATH%|$VFRB_EXEC_PATH/$VFRB_TARGET|" \
    -e "s|%VFRB_PROP_PATH%|$VFRB_PROP_PATH/$VFRB_PROP.properties|" \
    <$VFRB_ROOT/service/vfrb.service >$VFRB_NAME.service
echo ""
echo "$VFRB_NAME.service created in $VFRB_ROOT/target/service/"
echo ""
popd
echo ""
echo "... FINISHED ..."