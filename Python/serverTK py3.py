import socket as s
import tkinter as t
import _thread  

main = t.Tk()

com = s.socket(s.AF_INET6,s.SOCK_DGRAM)
            
main.title('Servidor do Felipe')

servidorON = False

porta = 8802

botao = t.Button(main, text="Iniciar")
lporta = t.Label(main, text="Porta:")
l1 = t.Label(main, text="Dados")
l2 = t.Label(main, text="Endereco")
cb = t.Checkbutton(main, text="gravar em arquivo")
cb.select()

tporta = t.Entry(main)
tporta.insert(0,"8802")
textbox = t.Listbox(main, yscrollcommand='1')
textbox1 = t.Listbox(main, yscrollcommand='1')
 

def escutar(nome, a):
    pos=1
    while(1):   
        a = com.recvfrom(1024)
        textbox.insert(pos,a[0])
        textbox1.insert(pos,a[1][0])
        pos=pos+1
        with open('out.dat', 'a+') as arquivo:
            arquivo.writelines(str(a[0]))
        
        
    
def iniciar():
    
    global servidorON

    if not servidorON:
        try:
            com.bind(("", porta))
            textbox.insert(1,"servidor iniciado")
            textbox1.insert(1,porta)
            _thread.start_new_thread(escutar,(0,0))
            botao['text'] = "parar"             
            servidorON = True
        except:
            textbox.insert(1,"erro, porta usada ou Firewall")
            
    else:
         
         com.close()      
         botao['text'] = "iniciar"
         servidorON = False
         main.quit()


    
    
botao.config(command=iniciar)


botao.grid (column=2 , row=1, pady=10, padx=10);
cb.grid(column=3 , row=1, pady=10, padx=10)
lporta.grid(column=2 , row=2);
tporta.grid (column=3 , row=2, pady=10);
l1.grid (column=3 , row=3, pady=1);
l2.grid (column=4 , row=3, pady=1);

textbox.grid(column=3 , row=4)
textbox1.grid(column=4 , row=4)
main.minsize(width=450, height=300)
main.mainloop()

