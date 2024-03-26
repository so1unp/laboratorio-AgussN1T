/*
  Escribe "Hola Mundo" en archivo ejwrite.dat
  Autor C.B.
*/

#include<string.h> 

int main(int argc, char* argv[])
{
  char buf[100];
  strcpy(buf,"Hola Mundo\n");
  //retorno del primer syscall
  int fd;
  fd = creat("ejwrite.dat",0640);
  //el tamaño del dato
  write(fd,buf,strlen(buf));
  close(fd);

  return 0;
}

