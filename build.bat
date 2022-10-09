cd ./compiler/
go build
cp ./main ../barn
rm ./main
cd ../
mkdir -p /.barn/libs/
cp -r ./lib/* /.barn/libs/