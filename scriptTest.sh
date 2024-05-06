cd bin
./client execute 300 -u ls -l
./client execute 5000 -u ./void 10
./client execute 400 -u ls -l
./client execute 1400 -p "./hello 7 | grep 1 | wc -l"
./client execute 5000 -u ./void 13
./client execute 1050 -p "./hello 5 | grep 1 | wc -l"
./client execute 5 -u date
./client execute 10 -p "echo "Digite seu texto aqui" | grep -o "palavra" | wc -l"
./client execute 2 -u cat /etc/passwd
./client execute 25 -p "seq 1 10 | grep '[02468]'"
./client execute 5 -u df
./client execute 30 -p "seq 1 10 | paste -sd+ | bc"
./client execute 7 -u free
./client execute 35 -p "echo "Tenho 3 maçãs e 5 bananas" | grep -o '[0-9]\+'"
./client execute 1 -u env
./client execute 6 -u env
./client execute 6 -u uname -a
./client execute 6 -u who
./client execute 6 -u du -h --max-depth=1
./client execute 5005 -u ./void 30
./client execute 6 -u date
./client execute 100 -p "echo "Isso é uma frase de exemplo" | wc -w"
./client execute 6 -u ./hello 1
./client execute 6 -u env
./client execute 10 -p "echo "Isso é uma frase de exemplo" | wc -c"
./client execute 6 -u who
./client execute 2500 -u ./void 20
./client execute 10 -p "echo "TEXTO" | tr '[:upper:]' '[:lower:]'"
./client execute 1000 -u ./void 10
./client execute 400 -u ls -l
./client execute 1030 -p "./hello 7 | grep 1 | wc -l"
./client execute 1000 -u ./void 13
./client execute 1200 -p "./hello 5 | grep 1 | wc -l"
./client execute 5 -u date
./client execute 7 -u free
./client execute 35 -p "echo "Tenho 3 maçãs e 5 bananas" | grep -o '[0-9]\+'"
./client execute 1 -u env
./client execute 6 -u uname -a
./client execute 6 -u who
./client execute 6 -u du -h --max-depth=1
./client execute 1500 -u ./void 30
./client execute 6 -u date
./client execute 100 -p "echo "Isso é uma frase de exemplo" | wc -w"
./client execute 6 -u ./hello 1
./client execute 6 -u env
./client execute 6 -u uname -a
./client execute 6 -u who
./client execute 6 -u du -h --max-depth=1
./client execute 2555 -u ./void 30
./client execute 6 -u date
./client execute 10 -p "echo "Digite seu texto aqui" | grep -o "palavra" | wc -l"
./client execute 6 -u who
./client execute 3450 -u ./void 20
./client execute 10 -p "echo "TEXTO" | tr '[:upper:]' '[:lower:]'"
./client execute 1000 -u ./void 10
./client execute 400 -u ls -l
./client execute 1000 -p "./hello 7 | grep 1 | wc -l"
./client execute 1000 -u ./void 13
./client execute 1030 -p "./hello 5 | grep 1 | wc -l"