check_arr=0
            last_i=
            #echo $out
            out=($out)
            echo ${out[0]}
            b_ifs=$IFS
            IFS=" "
            for o in $out
            do
                #echo "o : " $o
                if [[ check_arr%2 -eq 0 ]]
                then
                    echo -n $o " "
                    check_arr=$(( check_arr + 1 ))
                else
                    
                    check_arr=$(( check_arr - 1 ))
                fi
                #for i in $o
                #do
                #    if [[ check_arr%2 -eq 0 ]]
                #    then
                        #echo "check_arr : " $check_arr
                        #echo "i : " $i
                #        my_out[$i]=
                #        last_i=$i
                #    else
                #        echo "last_i : " $last_i " i : " $i
                #        my_out[$last_i]=$i
                #        echo "my_out : " ${my_out[z]}
                #    fi
                #    check_arr=$(( check_arr + 1 ))
                    #echo "o : " $o "|||"
                #done
                #IFS=b_ifs
            done
            #echo ${#my_out[0]}
            #out=($out)
            #echo ${out["1"]}
            #infected=$(( infected + out[0] ))
            #m=$(( m + out[1] ))
            #z=$(( z + out[2] ))