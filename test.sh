$1 > cmd.sh
bash < cmd.sh > og.txt
./minishell < cmd.sh > ans.txt
diff og.txt ans.txt
