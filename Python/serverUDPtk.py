 import socket as s
import Tkinter as t
import turtle
import thread  

main = t.Tk()

com = s.socket(s.AF_INET,s.SOCK_DGRAM)
            
main.title('Servidor')

servidorON = False

porta = 8080

botao = t.Button(main, text="Iniciar")
lporta = t.Label(main, text="Porta:")
tporta = t.Entry(main)
tporta.insert(0,"8080")
textbox = t.Listbox(main)

def escutar(nome, a):
    while(1):
        a = com.recvfrom(1024)
        textbox.insert(0,a[0])
        
    
def iniciar():
    
    global servidorON

    if not servidorON:
        try:
            com.bind(("", porta))
            textbox.insert(1,"servidor iniciado")
            thread.start_new_thread(escutar,(0,0))
            botao['text'] = "parar"             
            servidorON = True
        except:
            textbox.insert(1,"erro, porta usada ou Firewall")
            
    else:
         
         com.close()      
         botao['text'] = "iniciar"
         servidorON = False


    
    
botao.config(command=iniciar)


botao.grid (column=2 , row=1, pady=10, padx=10);
lporta.grid(column=2 , row=2);
tporta.grid (column=3 , row=2, pady=10);
textbox.grid(column=3 , row=3)
main.minsize(width=200, height=300)
main.mainloop()

