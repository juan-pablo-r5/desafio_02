import os
import random

# --- Listas de Palabras para Nombres ---
# (Puedes agregar más palabras a estas listas para mayor variedad)

ARTIST_PREFIX = [
    "The", "Crimson", "Electric", "Midnight", "Echo", "Starlight", 
    "Solar", "Velvet", "Lunar", "Silent", "Cosmic", "Lost", 
    "Golden", "Frozen", "Ancient", "Radio", "Neon"
]
ARTIST_SUFFIX = [
    "Wolves", "Voyagers", "Kings", "Rebels", "Giants", "Dreams", 
    "Shadows", "Echoes", "Riders", "Machine", "Youth", "Horizon", 
    "Saints", "Pilots", "Giants", "Nomads", "Band"
]

ALBUM_PREFIX = [
    "Songs of", "Echoes from", "Tales of", "Chronicles", "Memories", 
    "A Journey to", "The Art of", "Before the", "Under a", "Rhythm of",
    "Letters from", "The Fall of", "Ghosts of"
]
ALBUM_SUFFIX = [
    "Silence", "Yesterday", "The Void", "Innocence", "The Storm", 
    "Midnight", "The City", "The Sun", "Redemption", "Dust", 
    "Wonder", "Nowhere", "The Deep", "Home"
]

TRACK_PREFIX = [
    "Running", "Lost in", "Heart of", "Edge of", "Fading", "Broken", 
    "Whispers", "Dancing in the", "Forever", "One Last", "Chasing",
    "Beyond the", "Waking", "Still", "Waiting for"
]
TRACK_SUFFIX = [
    "Away", "Time", "Glass", "Tomorrow", "Light", "Dreams", "Rain", 
    "Night", "Goodbye", "Moment", "Stars", "Sun", "Up", "Burning", "You"
]

# --- Configuración de Nombres ---
base_directory = "music_dataset"  # Carpeta principal
cover_filename = "portada.png"    # Nombre de la portada
track_extension = ".wav"          # Extensión de las pistas
# ---------------------------------


def generate_random_name(list1, list2):
    """Combina dos palabras aleatorias de las listas provistas."""
    return f"{random.choice(list1)} {random.choice(list2)}"

def generate_music_data(num_artistas, max_albumes, max_canciones):
    """
    Genera un diccionario de datos de música aleatorio usando las listas de palabras.
    """
    print(f"Generando datos para {num_artistas} artistas aleatorios...")
    music_data = {}

    for _ in range(num_artistas):
        artist_name = generate_random_name(ARTIST_PREFIX, ARTIST_SUFFIX)
        
        if artist_name in music_data:
            continue

        albums = {}
        num_albumes = random.randint(1, max_albumes)

        for _ in range(num_albumes):
            album_title = generate_random_name(ALBUM_PREFIX, ALBUM_SUFFIX)
            
            tracks = []
            num_tracks = random.randint(5, max_canciones) # Mínimo 5 canciones

            for _ in range(num_tracks):
                track_title = generate_random_name(TRACK_PREFIX, TRACK_SUFFIX)
                # Asegurarse de que no haya títulos duplicados en el *mismo* álbum
                while track_title in tracks:
                    track_title = generate_random_name(TRACK_PREFIX, TRACK_SUFFIX)
                tracks.append(track_title)
            
            albums[album_title] = tracks
        
        music_data[artist_name] = albums

    print("¡Datos de artistas generados!")
    return music_data


def create_dataset_structure(base_dir, data):
    """
    Crea la estructura de carpetas y archivos vacíos con nombres de pistas.
    """
    print(f"Iniciando la creación del dataset en: '{base_dir}'")
    os.makedirs(base_dir, exist_ok=True)
    
    total_artistas = len(data)
    artista_actual = 0

    for artist, albums in data.items():
        artista_actual += 1
        print(f"  [Artista {artista_actual}/{total_artistas}] Creando: {artist}")
        
        artist_path = os.path.join(base_dir, artist)
        os.makedirs(artist_path, exist_ok=True)

        for album_title, track_titles in albums.items():
            album_path = os.path.join(artist_path, album_title)
            os.makedirs(album_path, exist_ok=True)
            # print(f"    [Álbum] Creado: {album_path}") # Descomentar para más detalle

            # 1. Crear portada
            cover_path = os.path.join(album_path, cover_filename)
            try:
                with open(cover_path, 'w') as f:
                    pass
            except IOError as e:
                print(f"      -> [Error] No se pudo crear {cover_path}: {e}")

            # 2. Crear pistas con nombres
            for i, track_title in enumerate(track_titles, 1):
                # Formato: "1. Título de la Canción.wav"
                track_name = f"{i}. {track_title}{track_extension}"
                track_path = os.path.join(album_path, track_name)
                
                try:
                    with open(track_path, 'w') as f:
                        pass
                except IOError as e:
                    # Esto podría pasar si el título tiene caracteres inválidos
                    # pero nuestras listas son seguras.
                    print(f"      -> [Error] No se pudo crear {track_path}: {e}")
            
            # print(f"      -> [Pistas] Creadas: {len(track_titles)}")

    print(f"\n¡Estructura del dataset generada exitosamente en '{base_dir}'!")

# --- Ejecutar el script ---
if __name__ == "__main__":
    
    # --- Controles de Generación ---
    # ¡Modifica estos números!
    
    NUM_ARTISTAS = 10            # ¿Cuántos artistas diferentes?
    MAX_ALBUMES_POR_ARTISTA = 2   # ¿Cuántos álbumes *como máximo* tendrá cada artista?
    MAX_CANCIONES_POR_ALBUM = 10  # ¿Cuántas canciones *como máximo* tendrá cada álbum?
    # ---------------------------------
    
    # 1. Genera el diccionario de datos aleatorio
    music_data = generate_music_data(
        NUM_ARTISTAS, 
        MAX_ALBUMES_POR_ARTISTA, 
        MAX_CANCIONES_POR_ALBUM
    )
    
    # 2. Crea la estructura de carpetas y archivos
    create_dataset_structure(base_directory, music_data)