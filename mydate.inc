read tz <<<$(date "+%:::z") 
LEAPOCH=$((946684800 + 86400*(31+29) ))
#DAYS_PER_400Y=$((365*400 + 97))
DAYS_PER_100Y=$((365*100 + 24))
DAYS_PER_4Y=$((365*4 + 1))
days_in_month=(31 30 31 30 31 31 30 31 30 31 31 29)
monthnames=(Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec)
shdate() {
#    secs=${EPOCHREALTIME%.*}
#    secs=$((startunix + seconds - LEAPOCH))
    secs=$(( ${EPOCHREALTIME%.*} - LEAPOCH + 3600 * $tz ))
    days=$(( secs / 86400 ))
    remsecs=$((secs % 86400))

#    wday=$(( (3 + days) % 7 ))
#    ((wday<0)) && ((wday+=7))

#    c_cycles=$((remdays / DAYS_PER_100Y))
#    remdays=$((days % DAYS_PER_100Y))
#    ((c_cycles==4)) && ((c_cycles--))
#    ((remdays-=(c_cycles*DAYS_PER_100Y) ))

    remdays=days
    q_cycles=$((remdays / DAYS_PER_4Y))
    ((q_cycles == 25)) && ((q_cycles--))
    ((remdays-=(q_cycles*DAYS_PER_4Y) ))

    remyears=$((remdays / 365))
    ((remyears==4)) && ((remyears--))
    ((remdays-=remyears*365 ))

    years=$((remyears + 4*q_cycles + 100*c_cycles + 400*qc_cycles))

    months=1
    while ((days_in_month[months-1]<=remdays)); do
        ((remdays-=days_in_month[months]))
        ((months++))
    done

    tm_year=$((years+2000))
    tm_mon=$((months + 2))
    if ((tm_mon>=12)); then
        ((tm_mon-=12))
        ((tm_year++))
    fi
    tm_mday=$((remdays + 1))
#    tm_wday=$((wday))

    hour=$((remsecs / 3600))
    minute=$((remsecs / 60 % 60))
    sec=$((remsecs % 60))
#    eval hour=$(($hour $tz))
#    ((hour>23)) && ((hour-=24))
    ((hour<10)) && hour=" "$hour
    ((minute<10)) && minute=0$minute
    ((sec<10)) && sec=0$sec
    ((tm_mon<10)) && tm_mon=0$tm_mon
    ((tm_mday<10)) && tm_mday=0$tm_mday
    echo "$tm_year ${monthnames[$tm_mon-1]} $tm_mday=$hour:$minute:$sec"
}
