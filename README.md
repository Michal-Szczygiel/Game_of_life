## Co to jest?
Automat komórkowy, [gra w życie](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life). Kompilacja, pobranie niezbędnych programów pomocniczych i generowanie filmu z symulacji są automatyczne.

## Jak się do tego zabrać?
- uruchom terminal w folderze projektu (pod Windowsem wymagane będzie użycie środowiska linuksowego - [WSL](https://docs.microsoft.com/en-us/windows/wsl/install)),
- utwórz folder '**FRAMES**' (do tego folderu zapisywane będą kolejne klatki symulacji),
- użyj komendy: '**make prepare**' (zostanie zainstalowany pakiet ffmpeg, konieczne potwierdzenie uprawnień administratora),
- następnie wpisz: '**make build**' (projekt zostanie skompilowany przy pomocy kompilatora gcc),
- finalnie: '**make run**' (program zostanie uruchomiony, folder FRAMES wypełni się klatkami symulacji, powstanie film z symulacją)

## Uwagi:
- make i gcc są niezbędne do skompilowania i uruchomienia projeku (są domyślnie zaistalowane),
- stan początkowy symulacji jest generowany losowo,
- program kompiluje się i uruchamia pod Linuksem, zdjęcia i film można oglądać z poziomu Windowsa,
- co zrobić kiedy film nie działa? Dziwne, powinien... (kwestia formatu)
- parametry w pliku Makefile można zmieniać:
    - OUTPUT_FOLDER - folder na klatki symulacji,
    - FRAMES_NUMBER - liczba generowanych klatek,
    - FRACTION_ALIVE - frakcja żywych komórek na początku symulacji,
    - MOVIE_NAME - nazwa generowanego filmu z symulacji,
    - FRAME_RATE - klatki/sekunde generowange filmu

## Przykładowy efekt działania programu:
![](https://github.com/Michal-Szczygiel/Game_of_life/blob/main/game_of_life.gif)
