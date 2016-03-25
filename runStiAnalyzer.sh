a=$1
b=${a%.root}
root -l -b <<EOF
.L StiAnalyzer.C
StiAnalyzer s
s.GetHifyFile("$1")
s.Loop()
EOF
open $b.pdf
