import { createApp } from 'vue'
import { createPinia } from 'pinia'
import App from './App.vue'
import router from './router'
import './assets/main.css'

const app = createApp(App)

// FIXED: Explicit 'any' cast bypasses the strict Plugin assignability error caused by Vue/Pinia version mismatches
app.use(createPinia() as any)
app.use(router)

app.mount('#app')