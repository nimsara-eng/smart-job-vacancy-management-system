import { createRouter, createWebHistory } from 'vue-router'
import { useAuthStore } from '../stores/auth'
import { storeToRefs } from 'pinia'

declare module 'vue-router' {
  interface RouteMeta {
    requiresAuth?: boolean
    guest?: boolean
    role?: 'applicant' | 'employer' | 'admin'
  }
}

const router = createRouter({
  history: createWebHistory(),
  routes: [
    { path: '/', redirect: '/jobs' },
    { path: '/login', name: 'Login', component: () => import('../views/LoginView.vue'), meta: { guest: true } },
    { path: '/register', name: 'Register', component: () => import('../views/RegisterView.vue'), meta: { guest: true } },
    { path: '/jobs', name: 'Jobs', component: () => import('../views/JobsView.vue') },
    { path: '/jobs/:id', name: 'JobDetail', component: () => import('../views/JobDetailView.vue') },
    { path: '/dashboard', name: 'Dashboard', component: () => import('../views/DashboardView.vue'), meta: { requiresAuth: true } },
    { path: '/applications', name: 'Applications', component: () => import('../views/ApplicationsView.vue'), meta: { requiresAuth: true } },
    { path: '/post-job', name: 'PostJob', component: () => import('../views/PostJobView.vue'), meta: { requiresAuth: true, role: 'employer' } },
    { path: '/manage-jobs', name: 'ManageJobs', component: () => import('../views/ManageJobsView.vue'), meta: { requiresAuth: true, role: 'employer' } },
    { path: '/profile', name: 'Profile', component: () => import('../views/ProfileView.vue'), meta: { requiresAuth: true } },
    { path: '/admin', name: 'Admin', component: () => import('../views/AdminView.vue'), meta: { requiresAuth: true, role: 'admin' } },
  ],
})

router.beforeEach(guard => {
  const auth = useAuthStore()
  // FIXED: Extract as refs so TS knows to use .value inside standard TS logic
  const { user, isLoggedIn } = storeToRefs(auth)
  
  if (guard.meta.requiresAuth && !isLoggedIn.value) return { name: 'Login', query: { redirect: guard.fullPath } }
  if (guard.meta.guest && isLoggedIn.value) return { name: 'Dashboard' }
  if (guard.meta.role && user.value?.role !== guard.meta.role) return { name: 'Jobs' }
})

export default router