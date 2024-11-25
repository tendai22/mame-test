#! /bin/sh
cat "$@" |
sed -n '
$b flush
/^[-~=*'\'']/b flush
/^    /b flush
/^$/{
:flush
    x
    s/^\n//
    s/\n/ /g
    /^$/!p
    s/.*//
    x
    p
    b
}
/^$/!{
    H
    b
}
'