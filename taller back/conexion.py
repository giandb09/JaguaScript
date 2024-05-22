from models import db 
from flask import Flask

app = Flask(__name__)

# Una uri (uniforme resource indetifier) de base de datos es una cadena de texto que especifica la ubicacion 
# y los detalles de conexion para la base de datos que queremos usar en nuestra aplicación
app.config["SQLALCHEMY_DATABASE_URI"] = "sqlite:///mis_notas.db"

app.config["SECRET_KEY"] = "HOLA123"

db. init_app(app) # inicializa la aplicacion flask con la configuracion de SLQAlchemy

with app.app_context():
    db.create_all()