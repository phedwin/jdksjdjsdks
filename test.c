#if 0 

int checkKey() {
    int ret = 0;
    const char *password = "jdjdskdjskjd";
	const char *salt = "$6$SALT123$";

	char *hashed_password = crypt(password, salt);

	if (hashed_password)
    

    return ret;

}

#endif
