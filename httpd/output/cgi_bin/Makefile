.PHONY:al
al:debug_cgi plus_cgi
debug_cgi:debug_cgi.c
	gcc -o $@ $^
plus_cgi:plus_cgi.c
	gcc -o $@ $^

.PHONY:output
output:
	cp debug_cgi ../output/htdocs/cgi_bin
	cp plus_cgi ../output/htdocs/cgi_bin
	cp plus.html ../output/htdocs/cgi_bin


.PHONY:clean
clean:
	rm debug_cgi plus_cgi
