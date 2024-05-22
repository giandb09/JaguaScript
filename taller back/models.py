from flask_sqlalchemy import SQLAlchemy

db = SQLAlchemy() # db se convierte en un objeto que nos permite interactuar con la base de datos

class Usuarios(db.Model): #db.model indica que usuarios es un modelo de datos gestinado por SQLachemy
    id = db.Column(db.Integer, primary_key=True)
    nombre = db.Column(db.String, nullable = False)
    apellido = db.Column(db.String, nullable = False)
    cedula = db.Column(db.Integer, nullable = False)
    correo = db.Column(db.Integer, nullable = False)


    def __init__(self, nombre, apellido, cedula, correo):
        self.nombre = nombre
        self.apellido = apellido
        self. cedula = cedula
        self. correo = correo

class Notas(db.Model): 
    id = db.Column(db.Integer, primary_key = True)
    fecha = db.Column(db.String(20), nullable=False)
    titulo = db.Column(db.String(40), nullable = False)
    descripcion = db.Column(db.String, nullable = False)
    usuario_id = db.Column(db.Integer, db.ForeignKey("usuarios.id"), nullable = False)

    def __init__(self, fecha, titulo, descripcion, usuario_id):
        self.fecha = fecha
        self.titulo = titulo
        self.descripcion = descripcion
        self.usuario_id = usuario_id