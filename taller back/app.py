from flask import render_template, request, redirect, url_for, session
from conexion import app, db 
from models import Usuarios, Notas 

@app.route("/")

def index():
    nombre = "Gian"
    return render_template ("index.html", nombre=nombre)

@app.route("/registrar", methods = ["POST", "GET"])

def registrar():
    if request.method == "POST":

        nombre = request.form ["nombre"]
        apellido = request.form ["apellido"]
        cedula = request.form ["cedula"]
        correo = request.form ["correo"]

        datos_usuario = Usuarios(nombre, apellido, cedula, correo)
        db.session.add(datos_usuario)

        db.session.commit()

        session["usuario_id"] = datos_usuario.id
        return render_template("cargar_notas.html")

    return render_template("registrar.html")
@app.route("/cargar_notas", methods = ["POST", "GET"])

def cargar_notas():

    if request.method == "POST":
        fecha = request.form ["fecha"]
        titulo = request.form ["titulo"]
        descripcion = request.form ["descripcion"]

        usuario_id = session.get("usuario_id")


        notas_registradas = Notas(fecha, titulo, descripcion, usuario_id)

        db. session.add(notas_registradas)
        db. session.commit()

        return render_template("cargar_notas.html")
    return render_template("cargar_notas.html")

@app.route("/ver_notas", methods = ["POST", "GET"])

def ver_notas():

    usuario_id = session.get("usuario_id")

    notas = Notas.query.filter_by(usuario_id=usuario_id).all()

    return render_template("ver_notas.html", notas=notas)

#Ruta para actualizar
@app.route("/actualizar_notas/<int:nota_id>", methods = ['POST', 'GET'])

def actualizar_notas(nota_id):

    # Creamos una variable para extraer los datos de la nota según el id 

    nota_a_actualizar = Notas.query.get(nota_id)

    # Recibimos los datos del formulario
    if request.method == "POST":
        fecha = request.form['fecha']
        titulo = request.form['titulo']
        descripcion = request.form['descripcion']

        # Modificamos los valores ya existentes

        nota_a_actualizar.fecha =  fecha
        nota_a_actualizar.titulo = titulo
        nota_a_actualizar.descripcion = descripcion

        # Confirmamos los cambios

        db.session.commit()

        # Enviamos al usuario a la ruta de ver_notas

        return redirect(url_for('ver_notas'))

    return render_template("actualizar_notas.html", nota_a_actualizar=nota_a_actualizar)

@app.route('/eliminar_nota', methods = ['GET', 'POST'])

def eliminar_nota():

    if request.method == 'POST':
        id = request.form['nota_id']
        nota_a_eliminar = Notas.query.filter_by(id=id).first()
        db.session.delete(nota_a_eliminar)

        db.session.commit()

        return redirect(url_for('ver_notas'))



if __name__ == "__main__":
    app.run (debug = True)