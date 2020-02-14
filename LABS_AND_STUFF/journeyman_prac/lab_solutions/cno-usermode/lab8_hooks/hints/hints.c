int printf_wrapper(const char* fmt, ...) {
	unsigned int maxlen;
	char* new_fmt;
	va_list args;
	int ret;
	char prefix[] = "[Hooked] ";

	maxlen = strlen(fmt) + sizeof(prefix);
	new_fmt = malloc(maxlen);
	if (new_fmt == NULL) {
		return -1;
	}
	snprintf(new_fmt, maxlen, "%s%s", prefix, fmt);
	va_start(args, fmt);
	ret = vprintf(new_fmt, args);
	va_end(args);
	free(new_fmt);
	return ret;
}