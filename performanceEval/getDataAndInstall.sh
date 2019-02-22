
# Installing metis
wget http://glaros.dtc.umn.edu/gkhome/fetch/sw/metis/metis-5.1.0.tar.gz
tar -xvzf metis-5.1.0.tar.gz
cd metis-5.1.0/build
MH=`pwd`
cd ..
make config prefix=$MH
make install
export METISROOT=$MH
cd ../



# Getting input matricies 
cd data/

wget https://sparse.tamu.edu/MM/Schenk_AFE/af_shell3.tar.gz
tar -xvzf af_shell3.tar.gz
mv af_shell3/af_shell3.mtx ./
rm -Rf af_shell3
rm af_shell3.tar.gz

wget https://sparse.tamu.edu/MM/GHS_psdef/bmwcra_1.tar.gz
tar -xvzf bmwcra_1.tar.gz
mv bmwcra_1/bmwcra_1.mtx ./
rm -Rf bmwcra_1
rm bmwcra_1.tar.gz

wget https://sparse.tamu.edu/MM/GHS_psdef/crankseg_2.tar.gz
tar -xvzf crankseg_2.tar.gz
mv crankseg_2/crankseg_2.mtx ./
rm -Rf crankseg_2
rm crankseg_2.tar.gz

wget https://sparse.tamu.edu/MM/DNVS/m_t1.tar.gz
tar -xvzf m_t1.tar.gz
mv m_t1/m_t1.mtx ./
rm -Rf m_t1
rm m_t1.tar.gz

wget https://sparse.tamu.edu/MM/INPRO/msdoor.tar.gz
tar -xvzf msdoor.tar.gz
mv msdoor/msdoor.mtx ./
rm -Rf msdoor
rm msdoor.tar.gz


# Building the performance drivers
cd ..
g++ -O3 -o performance performance.cc -std=c++11
make clean
cmake -DBUILDTYPE=release .
make


#running the driver
./performance execList.txt
