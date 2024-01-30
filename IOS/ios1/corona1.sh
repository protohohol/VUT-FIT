#!/bin/sh
set -o posix
usage() { 
echo "Usage: $0 [-h] [-abgsdrh] [COMMAND] [LOG [LOG2 [...]]]
-a DATETIME — after: jsou uvažovány pouze záznamy PO tomto datu (včetně tohoto data). DATETIME je formátu YYYY-MM-DD
-b DATETIME — before: jsou uvažovány pouze záznamy PŘED tímto datem (včetně tohoto data)
-g GENDER : jsou uvažovány pouze záznamy nakažených osob daného pohlaví. GENDER může být M (muži) nebo Z (ženy)
-s [WIDTH] : u příkazů gender, age, daily, monthly, yearly, countries, districts a regions vypisuje data ne číselně, 
             ale graficky v podobě histogramů. Nepovinný parametr WIDTH nastavuje šířku histogramů, tedy délku nejdelšího řádku, 
             na WIDTH. Tedy, WIDTH musí být kladné celé číslo. Pokud není parametr WIDTH uveden, řídí se šířky řádků požadavky 
             uvedenými níže
-d DISTRICT_FILE — pro příkaz districts vypisuje místo LAU 1 kódu okresu jeho jméno. Mapování kódů na jména je v souboru DISTRICT_FILE
-r REGIONS_FILE — pro příkaz regions vypisuje místo NUTS 3 kódu kraje jeho jméno. Mapování kódů na jména je v souboru REGIONS_FILE
-h — vypíše nápovědu s krátkým popisem každého příkazu a přepínače

COMMAND může být jeden z:
infected — spočítá počet nakažených
merge — sloučí několik souborů se záznamy do jednoho, zachovávající původní pořadí (hlavička bude ve výstupu jen jednou)
gender — vypíše počet nakažených pro jednotlivá pohlaví
age — vypíše statistiku počtu nakažených osob dle věku (bližší popis je níže)
daily — vypíše statistiku nakažených osob pro jednotlivé dny
monthly — vypíše statistiku nakažených osob pro jednotlivé měsíce
yearly — vypíše statistiku nakažených osob pro jednotlivé roky
countries — vypíše statistiku nakažených osob pro jednotlivé země nákazy (bez ČR, tj. kódu CZ)
districts — vypíše statistiku nakažených osob pro jednotlivé okresy
regions — vypíše statistiku nakažených osob pro jednotlivé kraje
"; exit 0; 
}

checkarg () {
if ! [[ $OPTARG =~ ^[0-9]+$ ]]
then
    OPTIND=$(( OPTIND - 1 ))
    s_arg=default
else
    s_arg="$OPTARG"
fi
}



a_flag=0
a_arg=
b_flag=0
b_arg=
g_flag=0
g_arg=
s_flag=0
s_arg=
d_flag=0
d_arg=
r_flag=0
r_arg=
z_flag=1

command=
logs=
l_z_flag=1 #logs zero flag

header="id,datum,vek,pohlavi,kraj_nuts_kod,okres_lau_kod,nakaza_v_zahranici,nakaza_zeme_csu_kod,reportovano_khs"
k_g=("M " "Z " "None") #keys for gender
k_a=("0-5 " "6-15 " "16-25 " "26-35 " "36-45 " "46-55 " "56-65 " "66-75 " "76-85 " "86-95 " "96-105 " ">105 " "None") #keys for age

infected=
m=
z=

input=
out=

eq_flag=0

IFS=$(printf '\n')

while getopts "a:b:g:s:d:r:h" arg;
do
    case "${arg}" in
        h)
            usage
            exit 0
            ;;
        a)
            a_flag=1
            a_arg="$OPTARG"
            z_flag=0
            ;;
        b)
            b_flag=1
            b_arg="$OPTARG"
            z_flag=0
            ;;
        g)
            g_flag=1
            g_arg="$OPTARG"
            z_flag=0
            ;;
        s)
            s_flag=1
            checkarg
            z_flag=0
            ;;
        d)
            d_flag=1
            d_arg="$OPTARG"
            z_flag=0
            ;;
        r)
            r_flag=1
            r_arg="$OPTARG"
            z_flag=0
            ;;

        *)
            echo "Wrong argument!"
            ;;
    esac
done

shift $(( OPTIND - 1 ))

while [ $# -gt 0 ]
do
    case "$1" in
        *.csv)
            logs+=" ${1}"
            in=$(cat ${1})
            input+=${in}
            if [ -n $l_z_flag ]
            then
                l_z_flag=0
            fi
            shift
            ;;
        *.gz)
            logs+=" ${1}"
            #input+=$($(gzip -cd ${1}) + "\n") #| cat /dev/stdin #input="$(< /dev/stdin)"
            #input+="$(< /dev/stdin)"
            if [ $l_z_flag -ne 0 ]
            then
                l_z_flag=0
            fi
            shift
            ;;
        *.bz2)
            logs+=" ${1}"
            if [ $l_z_flag -ne 0 ]
            then
                l_z_flag=0
            fi
            shift
            ;;
        *)
            if [ -z $command ]
            then
                if [ $l_z_flag -eq 0 ]
                then
                    echo "Wrong sequence of arguments!" >&2
                    exit 1
                fi
                command=$1
                shift
                z_flag=0
            else
                echo "Wrong argument : $1!" >&2
                exit 1
            fi
            ;;
        esac
done
echo $input