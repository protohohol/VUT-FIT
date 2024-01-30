if ($3 >= 0 && $3 < 6) {
                                out["0-5"]++
                            } else if ($3 > 5 && $3 < 16) {
                                out["6-15"]++
                            } else if ($3 > 15 && $3 < 26) {
                                out["16-25"]++
                            } else if ($3 > 25 && $3 < 36) {
                                out["26-35"]++
                            } else if ($3 > 35 && $3 < 46) {
                                out["36-45"]++
                            } else if ($3 > 45 && $3 < 56) {
                                out["46-55"]++
                            } else if ($3 > 55 && $3 < 66) {
                                out["56-65"]++
                            } else if ($3 > 65 && $3 < 76) {
                                out["66-75"]++
                            } else if ($3 > 75 && $3 < 86) {
                                out["76-85"]++
                            } else if ($3 > 85 && $3 < 96) {
                                out["86-95"]++
                            } else if ($3 > 95 && $3 < 106) {
                                out["96-105"]++
                            } else if ($3 > 105) {
                                out[">105"]++
                            } else {
                                out["None"]++
                            }

                            for (key in out) { print key " " out[key] }

                            IFS=$(printf '\n.'); IFS=${IFS%.}

                            IFS=$(printf '\n')
                            in=$(cat $log)
                            buf=($(echo $in | awk -F ',' \
                            -v a_f="$a_flag" \
                            -v a_a="$a_arg" \
                            -v b_f="$b_flag" \
                            -v b_a="$b_arg" \
                            -v g_f="$g_flag" \
                            -v g_a="$g_arg" \
                            -v s_f="$s_flag" \
                            -v s_a="$s_arg" \
                            -v d_f="$d_flag" \
                            -v d_a="$d_arg" \
                            -v r_f="$r_flag" \
                            -v r_a="$r_arg" \
                            -v z_f="$z_flag" \
                            -v header="$header" \
                            -v command="$command" \
                            -v k_g_b="${k_g[*]}" \
                            -v k_a_b="${k_a[*]}" \
                            'BEGIN { 
                                k_a_n = split(k_a_b, k_a, " ")
                                k_g_n = split(k_g_b, k_g, " ")
                                out["0-5"]=0
                                out["6-15"]=0
                                out["16-25"]=0
                                out["26-35"]=0
                                out["36-45"]=0
                                out["46-55"]=0
                                out["56-65"]=0
                                out["66-75"]=0
                                out["76-85"]=0
                                out["86-95"]=0
                                out["96-105"]=0
                                out[">105"]=0
                                out["None"]=0
                                err_c=0
                            }
                            {
                                check=1
                                orig=$0
                                gsub(" ", "")
                                if ($0 ~ header || $0 ~ /^[[:space:]]*$/) {
                                } else {
                                    if ($3 ~ /^[0-9]+$/) {
                                        month=substr($2, 6, 2)
                                        day=substr($2, 9, 2)
                                        if (day > "31" || day < "01") {
                                            err[err_c]="Invalid date: " orig
                                            err_c++
                                            check=0
                                        } else if (month > "12" || month < "01") {
                                            err[err_c]="Invalid date: " orig
                                            err_c++
                                            check=0
                                        } else if (month == "02") {
                                            if (day > "29") {
                                                err[err_c]="Invalid date: " orig
                                                err_c++
                                                check=0
                                            }
                                        }
                                    } else {
                                        err[err_c]="Invalid age: " orig
                                        err_c++
                                        check=0
                                    }
                                    if (check) {
                                        if (a_f) {
                                            if (a_a > $2) {
                                                check = 0
                                            }
                                        }
                                        if (b_f) {
                                            if (b_a < $2) {
                                                check = 0
                                            }
                                        }
                                        if (g_f) {
                                            if (g_a != $4) {
                                                check = 0
                                            }
                                        }
                                        if (check) {
                                            if (command == "infected") {
                                                if_f=1
                                                inf++
                                            }
                                            if (command == "gender") {
                                                gc_f=1
                                                if ($4 == "M") {
                                                    out["M"]++
                                                } else if ($4 == "Z") {
                                                    out["Z"]++
                                                } else {
                                                    out["None"]++
                                                }
                                            }
                                            if (command == "merge") {
                                                print
                                            }
                                            if (command == "age") {
                                                ac_f=1
                                                if ($3 >= 0 && $3 < 6) {
                                                    out["0-5"]++
                                                } else if ($3 > 5 && $3 < 16) {
                                                    out["6-15"]++
                                                } else if ($3 > 15 && $3 < 26) {
                                                    out["16-25"]++
                                                } else if ($3 > 25 && $3 < 36) {
                                                    out["26-35"]++
                                                } else if ($3 > 35 && $3 < 46) {
                                                    out["36-45"]++
                                                } else if ($3 > 45 && $3 < 56) {
                                                    out["46-55"]++
                                                } else if ($3 > 55 && $3 < 66) {
                                                    out["56-65"]++
                                                } else if ($3 > 65 && $3 < 76) {
                                                    out["66-75"]++
                                                } else if ($3 > 75 && $3 < 86) {
                                                    out["76-85"]++
                                                } else if ($3 > 85 && $3 < 96) {
                                                    out["86-95"]++
                                                } else if ($3 > 95 && $3 < 106) {
                                                    out["96-105"]++
                                                } else if ($3 > 105) {
                                                    out[">105"]++
                                                } else {
                                                    out["None"]++
                                                }
                                            }
                                            if (command == "daily") {
                                                print $2
                                            }
                                        }
                                    }
                                }
                            } END {
                                ORS=" "
                                if (gc_f) {
                                    for (i=0; i<=k_g_n; i++) {
                                        print k_g[i] " " out[k_g[i]]
                                    }
                                } else if (ac_f) {
                                    for (i=0; i<=k_a_n; i++) {
                                        print k_a[i] " " out[k_a[i]]
                                    }
                                } else if (if_f) {
                                    print inf
                                }
                                ORS="\n"
                                for (i=0; i<err_c; i++) {
                                    print err[i] >"/dev/stderr"
                                }
                            }'))

                                        -v s_f="$s_flag" \
            -v s_a="$s_arg" \
            -v d_f="$d_flag" \
            -v d_a="$d_arg" \
            -v r_f="$r_flag" \
            -v r_a="$r_arg" \
            -v z_f="$z_flag" \

             >&2 | awk '{print $0}'

             if (command == "age") {
        #if (s_f) {
        #}
        print "0-5   : " a_0
        print "6-15  : " a_6
        print "16-25 : " a_16
        print "26-35 : " a_26
        print "36-45 : " a_36
        print "46-55 : " a_46
        print "56-65 : " a_56
        print "66-75 : " a_66
        print "76-85 : " a_76
        print "86-95 : " a_86
        print "96-105: " a_96
        print ">105  : " a_105
        if (None) {
            print "None  : " None
        }
    }