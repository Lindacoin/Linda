#!/bin/bash
# create multiresolution windows icon
ICON_DST=../../src/qt/res/icons/Linda.ico

convert ../../src/qt/res/icons/Linda-16.png ../../src/qt/res/icons/Linda-32.png ../../src/qt/res/icons/Linda-48.png ${ICON_DST}
