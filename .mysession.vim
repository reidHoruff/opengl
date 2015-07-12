let SessionLoad = 1
if &cp | set nocp | endif
let s:so_save = &so | let s:siso_save = &siso | set so=0 siso=0
let v:this_session=expand("<sfile>:p")
silent only
if expand('%') == '' && !&modified && line('$') <= 1 && getline(1) == ''
  let s:wipebuf = bufnr('%')
endif
set shortmess=aoO
badd +5 ~/dev/opengl/p2/cube.c
badd +1 ~/dev/opengl/p2/make.sh
badd +1 lub/Angle.h
badd +17 ~/dev/opengl/lib/Angel.h
badd +14 ~/dev/opengl/p1/triangle.c
badd +402 ~/dev/opengl/lib/vec.h
badd +536 ~/dev/opengl/lib/mat.h
badd +189 ~/.vimrc
argglobal
silent! argdel *
edit ~/dev/opengl/p2/cube.c
set splitbelow splitright
set nosplitbelow
set nosplitright
wincmd t
set winheight=1 winwidth=1
argglobal
let s:l = 5 - ((4 * winheight(0) + 31) / 63)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
5
normal! 018|
if exists('s:wipebuf')
  silent exe 'bwipe ' . s:wipebuf
endif
unlet! s:wipebuf
set winheight=1 winwidth=20 shortmess=filnxtToOc
let s:sx = expand("<sfile>:p:r")."x.vim"
if file_readable(s:sx)
  exe "source " . fnameescape(s:sx)
endif
let &so = s:so_save | let &siso = s:siso_save
doautoall SessionLoadPost
unlet SessionLoad
" vim: set ft=vim :
