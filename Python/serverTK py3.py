import socket as s
import tkinter as t
import _thread  
import turtle as G

main = t.Tk()

com = s.socket(s.AF_INET6,s.SOCK_DGRAM)
            
main.title('Servidor do Felipe')

servidorON = False

x = 0
y= 0
porta = 8802

botao = t.Button(main, text="Iniciar")
lporta = t.Label(main, text="Porta:")
l1 = t.Label(main, text="Dados ASCII")
l2 = t.Label(main, text="Endereco")
l3 = t.Label(main, text="Temp. e Pressao ")
cb = t.Checkbutton(main, text="gravar em arquivo")
cb.select()

tporta = t.Entry(main)
tporta.insert(0,"8802")
textbox = t.Listbox(main, yscrollcommand='1')
textbox1 = t.Listbox(main, yscrollcommand='1')
textboxT = t.Listbox(main, yscrollcommand='1')
 



def escutar(nome, a):
    global x
    global G

    G.penup()
    G.pendown()

    pos=1
    while(1):   
        a = com.recvfrom(1024)
        textbox.insert(pos,a[0])
        textbox1.insert(pos,a[1][0])
       
        with open('out.dat', 'a+') as arquivo:
            arquivo.writelines(str(a[0]))

        b1 = a[0][1:6]
        b2 = a[0][6:len(a[0])]

        t = b1[3] | ( b1[4] << 8)
        
        print('temp:', t)
        print(b1)
        print(b2)

        
        p = (b2[2] <<0) | ( b2[3] << 8) << 2
 
        
        textboxT.insert(pos,str(t/100) +  "  " + str(int(p)))
        
        print('pressao', p)
        
        pos=pos+1 

        x=x+10
        
        G.goto(x,int(t/10))

                 
        
    
def iniciar():
    
    global servidorON

    if not servidorON:
        try:
            com.bind(("", porta))
            textbox.insert(1,"servidor iniciado")
            textbox1.insert(1,porta)
            textboxT.insert(1,000)
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
l3.grid (column=5 , row=3, pady=1);

textbox.grid(column=3 , row=4)
textbox1.grid(column=4 , row=4)
textboxT.grid(column=5 , row=4)

main.minsize(width=550, height=300)
main.mainloop()
