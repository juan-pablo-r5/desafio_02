import os
import random

# --- Listas de Palabras para Nombres ---
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
base_directory = "music_dataset"
cover_filename = "portada.png"
track_extension = ".wav"
# ---------------------------------


def generate_random_name(list1, list2):
    """Combina dos palabras aleatorias de las listas provistas."""
    return f"{random.choice(list1)} {random.choice(list2)}"

def create_dataset_with_ids(base_dir, num_artistas, max_albumes, max_canciones):
    """
    Genera la estructura completa de carpetas y archivos,
    incorporando el ID de 9 dígitos en el nombre del archivo.
    """
    print(f"Iniciando la creación del dataset en: '{base_dir}'")
    os.makedirs(base_dir, exist_ok=True)

    # Empezamos el contador de ID de artista en 10000 para asegurar 5 dígitos
    artist_id_counter = 10000

    # --- Loop de Artistas ---
    for i in range(num_artistas):
        artist_name = generate_random_name(ARTIST_PREFIX, ARTIST_SUFFIX)
        # ID de Artista (5 dígitos)
        artist_id_str = str(artist_id_counter)
        artist_id_counter += 1

        artist_path = os.path.join(base_dir, artist_name)
        os.makedirs(artist_path, exist_ok=True)
        print(f"  [Artista {artist_id_str}] Creando: {artist_name}")

        # --- Loop de Álbumes ---
        num_albumes = random.randint(1, max_albumes)
        for album_index in range(num_albumes):
            album_title = generate_random_name(ALBUM_PREFIX, ALBUM_SUFFIX)
            # ID de Álbum (2 dígitos)
            album_id_str = f"{(album_index + 1):02d}" # "01", "02", ...

            album_path = os.path.join(artist_path, album_title)
            os.makedirs(album_path, exist_ok=True)

            # Crear portada
            cover_path = os.path.join(album_path, cover_filename)
            try:
                with open(cover_path, 'w') as f: pass
            except IOError as e:
                print(f"      -> [Error] No se pudo crear {cover_path}: {e}")

            # --- Loop de Canciones ---
            track_titles = set() # Usamos un set para evitar duplicados fáciles
            num_tracks = random.randint(5, max_canciones)

            for track_index in range(num_tracks):
                track_title = generate_random_name(TRACK_PREFIX, TRACK_SUFFIX)
                while track_title in track_titles:
                    track_title = generate_random_name(TRACK_PREFIX, TRACK_SUFFIX)
                track_titles.add(track_title)

                # ID de Canción (2 dígitos)
                track_id_str = f"{(track_index + 1):02d}" # "01", "02", ...

                # --- Construcción del ID y Nombre de Archivo ---
                full_id_str = f"{artist_id_str}{album_id_str}{track_id_str}"
                track_name = f"{full_id_str} - {track_title}{track_extension}"
                track_path = os.path.join(album_path, track_name)

                try:
                    with open(track_path, 'w') as f:
                        pass
                except IOError as e:
                    print(f"      -> [Error] No se pudo crear {track_path}: {e}")

    print("\n¡Estructura del dataset generada exitosamente!")

# --- Ejecutar el script ---
if __name__ == "__main__":

    # --- Controles de Generación ---
    NUM_ARTISTAS = 10            # ¿Cuántos artistas diferentes?
    MAX_ALBUMES_POR_ARTISTA = 2   # ¿Cuántos álbumes *como máximo*?
    MAX_CANCIONES_POR_ALBUM = 10  # ¿Cuántas canciones *como máximo*?
    # ---------------------------------

    create_dataset_with_ids(
        base_directory,
        NUM_ARTISTAS,
        MAX_ALBUMES_POR_ARTISTA,
        MAX_CANCIONES_POR_ALBUM
    )
