#!/bin/sh
set -x
# This script builds a tarball for iegenlib
#
# First it copies all the files that will go into the tarball into 
# a directory based on the current date.
#
distdir=$(date "+iegenlib_%Y-%m-%d")
echo $distdir

rm -Rf $distdir
mkdir $distdir

# To copy the files, both the cp command and the rsync commands are used.
# The cp command does not have an exclusion options, so we use rsync
# to copy directories where we have files that need to be excluded, such
# as all the files in .svn.
#
# Copy the top-level files first, from the IEGen directory:
#
cp CMakeLists.txt configure COPYING Makefile README $distdir

# Use rsync to copy the doc directory - the CMakeLists and doxyconfig files 
# and the Python examples, minus the .svn directory:
#
rsync -r -L -p --exclude=".svn" --exclude="DoNotDistribute" doc/CMakeLists.txt doc/doxyconfig doc/PythonScriptExamples $distdir/doc

# Next copy the latex final pdf:
#
rsync -r -L -p doc/latex/refman.pdf $distdir/doc/

# This line does the same thing, only using a lot of exclusion strings so
# all the files generated in the latex directory aren't copied except for 
# the refman.pdf file.
#
#rsync -r -L -p --exclude="development*" --exclude="mapir*" --exclude="sparse_constraints*" --exclude=".svn" --exclude="*.tex" --exclude="*.md5" --exclude="*graph.pdf" --exclude="*incl.pdf" --exclude="*dep.pdf" --exclude="*.sty" --exclude="*.*x" --exclude="*.*i*" --exclude="*.*o*" --exclude="Makefile" doc $distdir

# Next copy the src directory, minus the .svn directories and the iegen 
# directory.
#
rsync -r -L -p --exclude=".svn" --exclude="iegen" --exclude="*.DS_Store" --exclude="*~" --exclude="*.out.dot" src $distdir

# Copy the Modules directory minus the .svn directory.
#
rsync -r -L -p --exclude=".svn" Modules $distdir

# Copy the binaries directory. Note that this won't work unless you
# change the name of the IEGen/iegen directory to IEGen/iegen-LINUX64 
# or IEGen/iegen-MacOSX, depending on which binaries you built.
#
rsync -r -L -p iegen-* $distdir

# These are the old lines: Everything was copied, then all the source
# control directories were removed. Additionally this script copied the
# scripts directory for some reason and ran the configure script but didn't
# do anything with the results. 
#cp -R CMakeLists.txt configure COPYING doc Makefile Modules README src $distdir
#rm -Rf $distdir/libs/gmp/.git $distdir/libs/isl/.git $distdir/libs/cloog/.git

# Finally tar it all up:
#
tar -cvzf $distdir.tgz $distdir

# Delete the directory used to create the tar file.
#rm -Rf $distdir
