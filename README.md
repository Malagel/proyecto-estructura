## Instalar WSL
1) En powershell con admin usar: wsl --install
2) Reiniciar PC cuando te diga.
3) Abrir Ubuntu en el buscador de Windows si no se abrió solo.
4) Crear usuario y contraseña. La contraseña no se ve porsiacaso.
5) Abrir VS Code y en Extensiones (Ctrl + Shift + X) descargar WSL.
6) Abrir terminal de Ubuntu en Windows (en el buscador) y escribir estos comandos en orden:

sudo apt update && sudo apt install build-essential git -y
mkdir universidad
cd universidad

## Github

1) Configurar usuario y correo en el terminal de Ubuntu con:

    git config --global user.name "Nombre Apellido"
    git config --global user.email "correo@gay.com"

2) Copiar el código de github con los siguientes comandos en el terminal:

    git clone https://github.com/Malagel/proyecto-estructura
    cd proyecto-estructura
    code . 

3) Se abrirá VS Code, dentro de él escribir este comando para autenticarse con GitHub:

    git fetch origin

## Flow general de trabajo:

1) Antes de trabajar en una nueva branch usar estos comandos para actualizar tu código:

    git checkout main
    git pull origin main

2) Después de actualizar el código, irse a la página de GitHub y escoger algún Issue pendiente, moverlo a In Progress y crear una nueva branch. Para moverte a la nueva branch usar:

    git fetch origin
    git checkout <nombre-branch>

3) Si mientras están trabajando en el código, se actualiza main, entonces usar estos comandos para actualizar el tuyo:

    git fetch origin
    git merge origin/main

4) Cuando terminen y hayan probado el código en su branch respectiva, usar estos comandos para subirlos a GitHub:

    git add .
    git commit -m "Algún comentario especificando qué hicieron"
    git push origin HEAD

5) El código ahora estará actualizado en la branch que crearon. Si están seguros que funciona bien y debería de unirse con el código principal deberán ir a GitHub (o apretar el link que les sale en el terminal después de hacer git push) para crear una Pull Request, que de ser aceptada se mezclará con el código en main.

6) En el mensaje del Pull Request poner una breve descripción de la implementación, y si el Issue ya está listo para ser cerrado, al final del comentario escribir "Closes #<NÚMERO DEL ISSUE A CERRAR>" Tal cual está, sin los "<>".


