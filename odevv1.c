/*
	***Movie Rental Console Application***
	
		1-	Team Lead(1): Berk Muammer Kuzu
		
		2-	Vice Lead(2): Muhammed Bucak
		
		3-	Team Member: Mustafa Berkay Aydın
		4-	Team Member:Yağmur Melisa Bakır 230201049
		5-	Team Member:Hatice Nur Uyanık 230201006
		6-	Team Member:Murat Dinçel 230201064
		7-	Team Member:Taima Adil Abdalla Taha 230202913
		8-	Team Member:
		9-	Team Member:
		10-	Team Member:
		
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_MOVIES 100
#define MAX_USERS 999
#define MAX_NAME 35
#define MAX_PASS 30
#define MAX_MOVIE_NAME 60

typedef struct {
    char title[MAX_MOVIE_NAME];
    int isRented;
    char rentedBy[MAX_NAME]; // HER KULLANICININ NE KİRALADIĞINI ÖZEL KAYDETMEK İÇİN MUHAMMED BUCAK
} Movie;

typedef struct {
    char username[MAX_NAME];
    char password[MAX_PASS];
    int isAdmin; // 1 for admin, 0 for regular user
} User;

Movie movies[MAX_MOVIES];
int movieCount = 0;

User users[MAX_USERS];
int userCount = 0;
int currentUserIndex = -1;

//////////////Functions Declarations///////////////
void saveMovies();
void loadMovies();
void loadUsers();
void saveUsers();
int login();
void registerUser();
void addMovie();
void listMovies();
void viewRentalHistory();
void rentMovie();
void returnMovie();
void menu();
void showComments();
//////////////////////////////////////////////////

//////////////////Main Function///////////////////
int main() {
	
    loadMovies();
    loadUsers();

    int choice;
	int i, exit1;
	
	puts("***********************************************");
	puts("********************Welcome********************\n***********************************************");
	puts("*********///* WHITE MOVIE RENTAL *///**********");
	puts("******************APPLICATION******************\n***********************************************");
	puts("***********************************************\n***********************************************\n\n"); 
	  
    do{
    	printf("**************\n1. Login\n2. Register\n3. Exit\nYour choice: ");
    	scanf("%d", &choice);
    	printf("**************\n\n");
    	
    	if (choice == 1 && login()) {
    		if (users[currentUserIndex].isAdmin) {
                printf("Welcome, Admin %s.\n", users[currentUserIndex].username);
            }
            if (users[currentUserIndex].isAdmin == 0){
            	printf("*******---\nWelcome, %s. ---\n*******\n", users[currentUserIndex].username);
			}
        	menu();
    	} else if (choice == 2) {
    	    registerUser();
    	    if (login()) menu();
    	} else if(choice == 3){
    	    return 0;
    	} else{
    		printf("Invalid input.\n");
		}
    	i++;
	}
	while (i != 10); // 10 kere art arda bu kısımda hata yaparsan program kapanıyor yada sürekli gir çık hesaba giriş yapmayı engellemek için
	
    return 0;
}
//////////////////////////////////////////////////

////////////Users-login-register part/////////////


//Muhammed Bucak Alınmış kullanıcı ismini tekrardan alırken uyarI verdirtiyor düzeltilmiş hali
void registerUser() {
    if (userCount >= MAX_USERS) {
        printf("User capacity reached.\n");
        return;
    }

    getchar(); // consume leftover newline from previous input

    int isUnique = 0;
    int i; // For loop counter defined outside

    while (!isUnique) {
        printf("New username: ");
        fgets(users[userCount].username, MAX_NAME, stdin);
        users[userCount].username[strcspn(users[userCount].username, "\n")] = '\0';

        // Check if username is unique
        isUnique = 1; // Assume unique
        for (i = 0; i < userCount; i++) {
            if (strcmp(users[i].username, users[userCount].username) == 0) {
                printf("Username already taken. Please choose another.\n");
                isUnique = 0;
                break;
            }
        }
    }

    printf("Password: ");
    fgets(users[userCount].password, MAX_PASS, stdin);
    users[userCount].password[strcspn(users[userCount].password, "\n")] = '\0';

    users[userCount].isAdmin = 0; // regular user by default
    userCount++;
    saveUsers();

    printf("Registration successful.\n");
}


int login() {
    char inputUser[MAX_NAME];
    char inputPass[MAX_PASS];
    int i, c;

    while ((c = getchar()) != '\n' && c != EOF);

    do {
        printf("Username: ");
        fgets(inputUser, MAX_NAME, stdin);
        inputUser[strcspn(inputUser, "\n")] = '\0';
        if (inputUser[0] == '\0')
            printf("Username cannot be empty. Please try again.\n");
    } while (inputUser[0] == '\0');

    do {
        printf("Password: ");
        fgets(inputPass, MAX_PASS, stdin);
        inputPass[strcspn(inputPass, "\n")] = '\0';
        if (inputPass[0] == '\0')
            printf("Password cannot be empty. Please try again.\n");
    } while (inputPass[0] == '\0');

    // MUHAMMED BUCAK DAHA ÖNCE ALINAN KULLANICI ADINI SORGULUYOR.
    for (i = 0; i < userCount; i++) {
        if (strcmp(inputUser, users[i].username) == 0 &&
            strcmp(inputPass, users[i].password) == 0) {
            printf("Login successful.\n");
            currentUserIndex = i;
            return 1;
        }
    }

    printf("Login failed.\n");
    return 0;
}


void saveUsers() {
	int i;
    FILE *f1 = fopen("usernames.txt", "w");
    FILE *f2 = fopen("passwords.txt", "w");
    FILE *f3 = fopen("roles.txt", "w");
    for (i = 0; i < userCount; i++) {
        fprintf(f1, "%s\n", users[i].username);
        fprintf(f2, "%s\n", users[i].password);
        fprintf(f3, "%d\n", users[i].isAdmin);
    }
    fclose(f1);
    fclose(f2);
    fclose(f3);
}

void loadUsers() {
    FILE *f1 = fopen("usernames.txt", "r");
    FILE *f2 = fopen("passwords.txt", "r");
    FILE *f3 = fopen("roles.txt", "r");
    if (!f1 || !f2 || !f3) return;

    while (1) {
        if (!fgets(users[userCount].username, MAX_NAME, f1)) break;
        if (!fgets(users[userCount].password, MAX_PASS, f2)) break;

        // Satır sonu karakterlerini temizle
        users[userCount].username[strcspn(users[userCount].username, "\n")] = '\0';
        users[userCount].password[strcspn(users[userCount].password, "\n")] = '\0';

        if (fscanf(f3, "%d\n", &users[userCount].isAdmin) != 1) break;
        userCount++;
    }

    fclose(f1);
    fclose(f2);
    fclose(f3);
}
//////////////////////////////////////////////////


/* MUHAMMED BUCAK
HANGİ KULLANICININ HANGİ FİLMİ KİRALADIĞINI KULLANICIYA ÖZEL MOVİE DOSYASINA KAYDEDİYOR RETURN İÇİN*/
void saveMovies() {
    FILE *f = fopen("movies.txt", "w");
    if (!f) {
        printf("Failed to open movies.txt for writing.\n");
        return;
    }
	int i;
    for (i = 0; i < movieCount; i++) {
        fprintf(f, "%s;%d;%s\n", movies[i].title, movies[i].isRented, movies[i].rentedBy);
    }

    fclose(f);
}

void loadMovies() {
    FILE *f = fopen("movies.txt", "r");
    if (!f) {
        printf("Failed to open movies.txt.\n");
        return;
    }

    char line[200]; // Her satır için bir buffer
    while (fgets(line, sizeof(line), f)) {
        char *token;
        token = strtok(line, ";");
        if (token) strcpy(movies[movieCount].title, token);

        token = strtok(NULL, ";");
        if (token) movies[movieCount].isRented = atoi(token);

        token = strtok(NULL, "\n");
        if (token) strcpy(movies[movieCount].rentedBy, token);
        else strcpy(movies[movieCount].rentedBy, ""); // Kiralayan yoksa boş bırak

        movieCount++;
    }

    fclose(f);
    printf("Movies loaded successfully. Total: %d\n", movieCount);
}

//////////// Yorum ekleme fonksiyonu Hatice Nur Uyanık 230201006 - Murat Dinçel 230201064 - Yağmur Melisa Bakır 230201049//////////////////////////
void addComment(const char* movieTitle, const char* username) {
    char comment[200];
    int wordCount = 0;

    // Kullanıcıya yorum eklemek isteyip istemediğini soruyoruz
    printf("Do you want to add a comment? (0: No, 1: Yes): ");
    int choice;
    scanf("%d", &choice);
    getchar(); // Kalan yeni satır karakterini alıyoruz

    // Eğer kullanıcı 1 girerse, yorum almak için işlemi başlatıyoruz
    if (choice == 1) {
        printf("Enter your comment (max 20 words): ");
        fgets(comment, sizeof(comment), stdin);

        // Yorumun 20 kelimeyi geçip geçmediğini kontrol ediyoruz
        char* token = strtok(comment, " ");
        while (token != NULL && wordCount < 20) {
            wordCount++;
            token = strtok(NULL, " ");
        }

        // Eğer yorum 20 kelimeden fazla ise kullanıcıya hata mesajı gösteriyoruz
        if (wordCount > 20) {
            printf("Your comment cannot exceed 20 words.\n");
            return;
        }

        // Yorum dosyaya kaydediliyor
        FILE* f = fopen("comment.txt", "a");
        if (f == NULL) {
            printf("Unable to open the comment file.\n");
            return;
        }
        // Yorum, film adı ve kullanıcı adıyla birlikte dosyaya yazılıyor
        fprintf(f, "%s;%s;%s\n", movieTitle, username, comment);
        fclose(f);
        // Yorum başarıyla kaydedildi mesajı
        printf("Comment has been successfully saved.\n");
    }
}

// Taima Adil Abdalla Taha - 230202913

// Function to show comments from comment.txt

void showComments() {

    char line[300];

    // Try to open the comment file in read mode

    FILE* f = fopen("comment.txt", "r");

    // If the file doesn't exist or can't be opened

    if (!f) {

        printf("No comments found.\n");

        return;

    }

    // Print all comments from the file

    printf("\n--- Comments ---\n");

    while (fgets(line, sizeof(line), f)) {

        printf("%s", line);

    }

    // Close the file after reading

    fclose(f);

}

void addMovie() {
	int c;
	
    if (!users[currentUserIndex].isAdmin) {
        printf("Only admins can add movies.\n");
        return;
    }
    if (movieCount >= MAX_MOVIES) {
        printf("Movie capacity reached.\n");
        return;
    }
    printf("Movie title: ");
    // consume leftover newline
    while ((c = getchar()) != '\n' && c != EOF);
    fgets(movies[movieCount].title, MAX_MOVIE_NAME, stdin);
    movies[movieCount].title[strcspn(movies[movieCount].title, "\n")] = '\0';
    movies[movieCount].isRented = 0;
    
    movieCount++;
    saveMovies();
    printf("Movie added.\n");
}

void listMovies() {
	int i;
    printf("\n--- Movie List ---\n");
    for (i = 0; i < movieCount; i++) {
        printf("%d. %s [%s]\n", i + 1, movies[i].title, movies[i].isRented ? "Rented" : "Available");
    }
}

// MUHAMMED BUCAK HANGİ KULLANICI HANGİ FİLMİ KİRALADI YA DA HANGİ FİLMİ İADE ETTİ rental_history.txt DOSYASINA SAAT VE TARİH OLARAK KAYIT ETTİRİYOR
void saveRentalHistory(const char* username, const char* title, const char* action) {
    FILE *f = fopen("rental_history.txt", "a");
    if (!f) {
        printf("Error: Unable to open rental history file.\n");
        return;
    }

    // Get current date and time
    time_t currentTime = time(NULL);
    struct tm *localTime = localtime(&currentTime);

    char dateTime[50];
    strftime(dateTime, sizeof(dateTime), "%Y-%m-%d %H:%M:%S", localTime);

    // Save to file with date and time
    fprintf(f, "%s;%s;%s;%s\n", username, title, action, dateTime);
    fclose(f);
}

//MUHAMMED BUCAK KİŞİYE ÖZEL MEÜ 5 ÇALIŞMASI
//HER KULLANICI KENDİ KİRALADIĞI FİLMLERİ GÖREBİLİYOR DİĞER KULLANICILAR GÖREMİYOR
void viewRentalHistory() {
    char line[300];
    FILE *f = fopen("rental_history.txt", "r");
    if (!f) {
        printf("No rental history available.\n");
        return;
    }

    printf("\n--- Your Rental History ---\n");
    while (fgets(line, sizeof(line), f)) {
        char *token = strtok(line, ";");
        char username[MAX_NAME], title[MAX_MOVIE_NAME], action[20], dateTime[50];

        if (token) strcpy(username, token);
        token = strtok(NULL, ";");
        if (token) strcpy(title, token);
        token = strtok(NULL, ";");
        if (token) strcpy(action, token);
        token = strtok(NULL, "\n");
        if (token) strcpy(dateTime, token);

        if (strcmp(username, users[currentUserIndex].username) == 0) {
            printf("Movie: %s\nAction: %s\nDate: %s\n\n", title, action, dateTime);
        }
    }

    fclose(f);
}

/* MUHAMMED BUCAK 
FİLMİ KULLANICI KİRALARKEN BİLGİSAYARIN SAATİNİ ALIYOR. 
KULLANICININ GİRDİĞİ SAATİ BU SAATE EKLEYİP KULLAICIYA KİRALAMA ZAMANINI VE TESLİM GÜNÜNÜ SUNUYOR */

void rentMovie() {
    int choice, duration;
    listMovies();
    printf("Enter the number of the movie you want to rent: ");
    scanf("%d", &choice);

    if (choice < 1 || choice > movieCount) {
        printf("Invalid selection.\n");
        return;
    }

    if (movies[choice - 1].isRented) {
        printf("This movie is already rented.\n");
    } else {
        do {
            printf("Enter rental duration (1-30 days): ");
            scanf("%d", &duration);

            if (duration < 1 || duration > 30) {
                printf("Invalid duration. Please enter between 1 and 30 days.\n");
            }
        } while (duration < 1 || duration > 30);

        // Mark movie as rented and assign rentedBy
        movies[choice - 1].isRented = 1;
        strcpy(movies[choice - 1].rentedBy, users[currentUserIndex].username);
        saveMovies();
        saveRentalHistory(users[currentUserIndex].username, movies[choice - 1].title, "Rented");

        // Get current date and time
        time_t currentTime = time(NULL);
        struct tm *rentalDate = localtime(&currentTime);
        
        // Print the rental date
        printf("\nMovie '%s' successfully rented.\n", movies[choice - 1].title);
        printf("Rental Date: %02d-%02d-%04d %02d:%02d:%02d\n",
               rentalDate->tm_mday, rentalDate->tm_mon + 1, rentalDate->tm_year + 1900,
               rentalDate->tm_hour, rentalDate->tm_min, rentalDate->tm_sec);

        // Calculate return date
        rentalDate->tm_mday += duration;
        mktime(rentalDate); // Normalize the date

        // Print the return date
        printf("Return Date: %02d-%02d-%04d %02d:%02d:%02d\n\n",
               rentalDate->tm_mday, rentalDate->tm_mon + 1, rentalDate->tm_year + 1900,
               rentalDate->tm_hour, rentalDate->tm_min, rentalDate->tm_sec);
    }
}




/* MUHAMMED BUCAK 
RETURN EDERKEN HER KULLANICI DİĞER KİŞİLERİN KİRALADIĞI FİLMLERİ İADE EDEBİLİYORDU. DÜZELTİLDİ
HER KULLANICI KENDİ KİRALADIKLARINI GÖRÜP İADE EDEBİLİYOR*/

void returnMovie() {
    int choice, found = 0;
    printf("\n--- Your Rented Movies ---\n");
	int i;
    for (i = 0; i < movieCount; i++) {
        if (movies[i].isRented && strcmp(movies[i].rentedBy, users[currentUserIndex].username) == 0) {
            printf("%d. %s\n", i + 1, movies[i].title);
            found = 1;
        }
    }

    if (!found) {
        printf("You have no rented movies.\n");
        return;
    }

    printf("Enter the number of the movie you want to return: ");
    scanf("%d", &choice);

    if (choice < 1 || choice > movieCount || strcmp(movies[choice - 1].rentedBy, users[currentUserIndex].username) != 0) {
        printf("Invalid selection.\n");
        return;
    }

    movies[choice - 1].isRented = 0;
    strcpy(movies[choice - 1].rentedBy, "");
    saveMovies();
    saveRentalHistory(users[currentUserIndex].username, movies[choice - 1].title, "Returned");
    printf("Movie successfully returned.\n");

/////// Film iade edildikten sonra yorum ekleme fonksiyonunu çağırıyoruz Hatice Nur Uyanık 230201006//////////////////////
    addComment(movies[choice - 1].title, users[currentUserIndex].username);
}



void menu() {
    int choice;
    do {
        printf("\n*****************\n1. List Movies\n2. Rent Movie\n3. Return Movie\n4. Add Movie (Admin Only)\n5. View Rental History\n6. Show the comments\n7. Exit\n");
        printf("\nYour choice: ");
        scanf("%d", &choice);
        printf("*****************\n"); 
        switch (choice) {
            case 1: listMovies(); break;
            case 2: rentMovie(); break;
            case 3: returnMovie(); break;
            case 4: addMovie(); break;
            case 5: viewRentalHistory(); break;
            case 6: showComments(); break; /////// Yorumları Göster Murat Dinçel 230201064 //////////
            case 7: printf("Logout\n\n\n"); break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 7);
}